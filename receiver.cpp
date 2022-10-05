#include "receiver.hpp"

#include <cstdlib>
#include <iostream>
#include <thread>

extern "C" {
    #include "fake_receiver.h"  
}

#define DUMP_LOG_FILENAME "candump.log"

using namespace std;

#ifdef DEBUG
    void log(CanMessage *message) {  
        cout << "Receiver -> length: " << message->length << " data: " << message->data << endl;
    }
#else
    void log(CanMessage *message) {}
#endif

Receiver::Receiver(MessageQueue *_queue) {

    queue = _queue;

    int openedSuccesfully = open_can("candump.log"); 
    if (openedSuccesfully != 0) {
        cout << "Error: the file doesn't exists or is already opened.";
    }

    // ? The production algorithm must implement the exception.
}

void Receiver::run() {
    int received;

    while (true) {
        CanMessage* message = new CanMessage;
        memset(message->data, 0, MAX_CAN_MESSAGE_SIZE);
        received = can_receive(message->data);
        
        // if can_message returns an error message reciver must go down.
        if (received == -1) return;
        
        message->length = received;
        message->timestamp = time(nullptr);

        queue->enqueue(*message);
        log(message);
    }
}

void Receiver::start() {
    thr = new thread(&Receiver::run, this);
}

Receiver::~Receiver() {
    close_can();
}

void Receiver::join() {
    thr->join();
}