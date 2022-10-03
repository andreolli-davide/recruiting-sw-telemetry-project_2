#pragma once

#include "queue.hpp"

extern "C" {
    #include "fake_receiver.h"
}

#define DUMP_LOG_FILENAME "candump.log"

class Receiver {
    private: 
        MessageQueue *queue;
        char message[MAX_CAN_MESSAGE_SIZE];
        int received;
        void run();
    public:
        thread* thr;
        Receiver(MessageQueue *_queue);
        ~Receiver();
        void (*log)(int*, char*);
        void start();
};