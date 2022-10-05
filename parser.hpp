#pragma once

#include <fstream>
#include "queue.hpp"
#include "common.hpp"

extern "C" {
    #include "fake_receiver.h"
}

class Parser {
    private:
        /**
         *  _idle is the handler for messages in Idle state.
         */
        void _idle(CanMessage *message);

        /**
         *  _run is the handler for messages in Rnu state.
         */
        void _run(CanMessage *message);

        MessageQueue *queue;

        /**
         *  message stores the actual message handled.
         */ 
        CanMessage* message;

        /**
         *  action stores the actual handler based of state (_idle in Idle mode, _run in Run mode).
         */
        void (Parser::*action)(CanMessage *message);
        
        /**
         *  file where session is saved.
         */
        ofstream file;

        char filename[24];
        thread* thr;
        
        /**
         *  run method does the work of the parser. It can called only by the public start() method.
         */
        void run();
    public:

        /**
         *  Parser constructor need to get the queue as parameter. The queue is shared with Reciver thread.
         */
        Parser(MessageQueue *_queue);

        /**
         *  Parser destructor closes the file before process termination.
         */ 
        ~Parser();

        /**
         *  Parser creates a thread with run function. 
         */
        void start();
        
        /**
         *  Join the Parser thread with function caller thread.
         */
        void join();
};