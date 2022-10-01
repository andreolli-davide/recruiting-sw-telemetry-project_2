#include "fake_receiver.h"
#include <iostream>

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
        uint64_t _size = 0;
        char *buffer;
        mutex mtx;
        condition_variable cv;
    public:
        /**
         *  Class constructor, it doesn't need any.
         */
        MessageQueue();

        /**
         *  Add a message to the queue.
         */
        void enqueue(char message[MAX_CAN_MESSAGE_SIZE]);

        /**
         *  Get and remove from queue the oldest message enqueued.
         */
        char* dequeue();

        /**
         *  Get the number of messages in the queue.
         */
        uint16_t size();
};

/**
 *  MessageQueueFullException can be throwed by MessageQueue's method enqueue while inserting an element into a full queue.
 */
class MessageQueueFullException : public exception {
    uint64_t size = MAX_QUEUE_SIZE;
    public:
        string getMessage();
};