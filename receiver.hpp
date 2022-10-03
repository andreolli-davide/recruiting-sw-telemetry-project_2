#pragma once

#include <chrono>
#include <ctime>
#include "queue.hpp"

extern "C" {
    #include "fake_receiver.h"
}

#define DUMP_LOG_FILENAME "candump.log"

using namespace std;

class Receiver {
    private: 
        MessageQueue *queue;
        CanMessage message;
        void run();
    public:
        thread* thr;
        Receiver(MessageQueue *_queue);
        ~Receiver();
        void start();
};