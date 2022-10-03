#include <mutex>
#include <condition_variable>
#include <iostream>
#include "queue.hpp"

extern "C" {
    #include "fake_receiver.h"
}

MessageQueue::MessageQueue() {
    buffer = new char[MAX_CAN_MESSAGE_SIZE * MAX_QUEUE_SIZE];
}

void MessageQueue::enqueue(char message[MAX_CAN_MESSAGE_SIZE]) {

    unique_lock<mutex> lock(mtx);

    if (_size >= MAX_QUEUE_SIZE) throw new MessageQueueFullException();

    strcpy(&buffer[((pointer + _size) % MAX_QUEUE_SIZE) * MAX_CAN_MESSAGE_SIZE], message);
    _size++;

    mtx.unlock();

    cv.notify_one();
}

char* MessageQueue::dequeue() {

    while (size() == 0) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock);
    }

    {
        scoped_lock lock(mtx);
        pointer++;
        _size--;
    }

    return &buffer[((pointer - 1) % MAX_QUEUE_SIZE) * MAX_CAN_MESSAGE_SIZE];
}

uint16_t MessageQueue::size() {

    unique_lock<mutex> lock(mtx);
    return _size;
}

string MessageQueueFullException::getMessage() {
    return "MessageQueue is full. Unable to enqueue the new message";
}