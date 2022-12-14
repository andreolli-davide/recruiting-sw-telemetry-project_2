#include <mutex>
#include <condition_variable>
#include <iostream>
#include <unistd.h>
#include "queue.hpp"
#include "common.hpp"

extern "C" {
    #include "fake_receiver.h"
}

MessageQueue::MessageQueue() {
    _size = 0;
    stop = false;
}
void MessageQueue::enqueue(CanMessage message) {

    unique_lock<mutex> lock(mtx);

    // Check if queue is full
    if (_size >= MAX_QUEUE_SIZE) throw new MessageQueueFullException();

    buffer[((pointer + _size) % MAX_QUEUE_SIZE)] = message;
    _size++;

    mtx.unlock();

    cv.notify_one();
}

CanMessage* MessageQueue::dequeue() {

    while (size() == 0 && !stop) {
        unique_lock<mutex> lock(mtx);
        if (stop)
            return nullptr;
        cv.wait(lock);
    }

    {
        scoped_lock lock(mtx);
        pointer++;
        _size--;
    }

    return &buffer[((pointer - 1) % MAX_QUEUE_SIZE)];
}

uint16_t MessageQueue::size() {

    unique_lock<mutex> lock(mtx);
    return _size;
}

void MessageQueue::stopDequeue() {
    stop = true;
    if (_size == 0) {
        mtx.unlock();
        cv.notify_one();
    }
}

string MessageQueueFullException::getMessage() {
    return "MessageQueue is full. Unable to enqueue the new message";
}