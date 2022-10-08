#include <iostream>
#include <thread>
#include <stdlib.h>

#include "parser.hpp"
#include "queue.hpp"
#include "statistics.hpp"

using namespace std;

// If env variable DEBUG is setted (any value), logs are printed on the console.
#ifdef DEBUG
    void log(char* mode, char* data) {  
        cout << "Parser -> data: " << data << endl;
    }
#else
    void log(char* mode, char* data) {}
#endif

Parser::Parser(MessageQueue *_queue) {
    queue = _queue;

    // Default action
    action = &Parser::_idle;
}

Parser::~Parser() {

    // Close session file before destroy
    if (file.is_open()) file.close();
}

void Parser::run() {
    while (true) {

        // Get message or wait that reciver enqueue something.
        message = queue->dequeue();

        // Manage the message with actual state handler.
        (*this.*action)(message);
    }
}

void Parser::start() {
    thr = new thread(&Parser::run, this);
}

void Parser::_idle(CanMessage *message) {

    // Check the first part of the data (0A0) before checking the entire string
    if (strncmp(message->data, "0A0", 3) == 0 && 
        (strncmp(&message->data[4], "6601", 4) == 0 || strncmp(&message->data[4], "FF01", 4) == 0)) {
                    
            action = &Parser::_run;
            sprintf(filename, "./%d_SESSION.txt", message->timestamp);
            file.open(filename);

            stats.init(message->timestamp);
            stats.report("0A0", message->timestamp);
    }

    log("idle", message->data);
}

void Parser::_run(CanMessage *message) {

    file << message->timestamp << " " << message->data << endl;
    
    // Add the message to statistics
    char id[3];
    strncpy(id, message->data, 3);

    stats.report(id, message->timestamp);
    
    // Check the first part of the data (0A0) before checking the entire string
    if (strncmp(message->data, "0A0", 3) == 0 && strncmp(&message->data[4], "66FF", 4) == 0) {
        file.close();
        stats.build();
        action = &Parser::_idle;
        return;
    }

    log("run", message->data);
}

void Parser::join() {
    thr->join();
}