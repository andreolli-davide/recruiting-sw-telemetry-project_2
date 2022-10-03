#include "receiver.hpp"

#include <cstdlib>
#include <iostream>
#include <thread>

extern "C" {
    #include "fake_receiver.h"  
}

#define DUMP_LOG_FILENAME "candump.log"

using namespace std;

void _noLog(int* id, char* payload) {}

void _debugLog(int* id, char* payload) {
    cout << "Receiver -> id: " << *id << ", payload: " << payload << endl;
}

Receiver::Receiver(MessageQueue *_queue) {

    // Setup log function if the env DEBUG is setted
    if(getenv("DEBUG")) 
        log = &_debugLog;
    else log = &_noLog;

    queue = _queue;

    int openedSuccesfully = open_can("candump.log"); 
    if (openedSuccesfully != 0) {
        cout << "Error: the file doesn't exists or is already opened.";
    }

    // TODO: Create exception
}

void Receiver::run() {
    while (true) {
        received = can_receive(message);
        if (received != -1) {
            queue->enqueue(message);
            log(&received, message);
        }
    }
}

void Receiver::start() {
    thr = new thread(&Receiver::run, this);
}

Receiver::~Receiver() {
    close_can();
}