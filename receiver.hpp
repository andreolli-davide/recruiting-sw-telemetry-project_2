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

        /**
         *  Actual message handled.
         */
        CanMessage message;

        /**
         *  run method does the work of the parser. It can called only by the public start() method.
         */
        void run();

        thread* thr;
    public:

        /**
         *  Receiver constructor need to get the queue as parameter. The queue is shared with Parser thread.
         */
        Receiver(MessageQueue *_queue);

        /**
         *  Receiver destructor closes the can stream before process termination.
         */ 
        ~Receiver();

        /**
         *  Receiver creates a thread with run function. 
         */
        void start();

        /**
         *  Join the Parser thread with function caller thread.
         */
        void join(); 
};