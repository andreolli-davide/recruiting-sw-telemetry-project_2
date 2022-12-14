#pragma once

#include "common.hpp"
#include <thread>
#include <exception>

extern "C" {
    #include "fake_receiver.h"
}

// sizeof CanMessage = 20 bytes --> memory space used by queue: 20 bytes * MAX_QUEUE_SIZE.
#define MAX_QUEUE_SIZE 10000

using namespace std;

/**
 *  MessageQueue class allows to split the work of receiving data from CAN bus and the parsing part in multiple threads.
 *  It is useful because the software must not lose incoming messages. So the heavy part of the work is done by 
 *  another thread, which can parse the data without interrupting the reciver.
 *  MessageQueue uses FIFO method, so the first message enqueued will also be the first to be processed by the parser.
 *  It uses a mutex for mutual exclusion and there shouldn't be problems with synchronized memory accesses.
 */
class MessageQueue {
    private:
        uint64_t pointer;
        uint64_t _size;
        CanMessage buffer[MAX_QUEUE_SIZE];
        mutex mtx;
        condition_variable cv;
        bool stop;
    public:
        /**
         *  Class constructor, it doesn't need any parameter.
         */
        MessageQueue();

        /**
         *  Add a message to the queue.
         */
        void enqueue(CanMessage message);

        /**
         *  Get and remove from queue the oldest message enqueued.
         */
        CanMessage* dequeue();

        /**
         *  Get the number of messages in the queue.
         */
        uint16_t size();

        void stopDequeue();
};

/**
 *  MessageQueueFullException can be throwed by MessageQueue's method enqueue while inserting an element into a full queue.
 */
class MessageQueueFullException : public exception {
    public:
        string getMessage();
};