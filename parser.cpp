#include <iostream>
#include <thread>
#include <stdlib.h>

#include "parser.hpp"
#include "queue.hpp"
#include "statistics.hpp"

using namespace std;

void parseBytes(CanMessage *message);

// If env variable DEBUG is setted (any value), logs are printed on the console.
#ifdef DEBUG
    void log(CanMessage *message, char* mode) {
        char payload[MAX_CAN_MESSAGE_SIZE / 2 - 2];
        sprintf(payload, "%02x", message->payload);
        cout << "Parser -> id: " << hex << message->id << ", mode: " << mode << endl;
    }
#else
    void log(CanMessage *message, char* mode) {}
#endif

Parser::Parser(MessageQueue *_queue) {
    queue = _queue;

    // Default action
    action = &Parser::_idle;

    returnOnEmptyQueue = [](CanMessage *message){};
}

Parser::~Parser() {

    // Close session file before destroying it
    if (file.is_open())
        file.close();
    exit(0);
}

void Parser::run() {
    while (true) {

        // Get message or wait that reciver enqueue something.
        message = queue->dequeue();
        returnOnEmptyQueue(message);

        parseBytes(message);

        // Manage the message with actual state handler.
        (*this.*action)(message);
    }
}

void Parser::start() {
    thr = new thread(&Parser::run, this);
}

void Parser::_idle(CanMessage *message) {

    // Check the first part of the data (0A0) before checking the entire string
    /*if (strncmp(message->data, "0A0", 3) == 0 && 
        (strncmp(&message->data[4], "6601", 4) == 0 || strncmp(&message->data[4], "FF01", 4) == 0)) {
            */
    
    if (message->id == 0xA0 && 
            (message->payload[0] == (byte)0x66 && message->payload[1] == (byte)0x1) ||
            (message->payload[0] == (byte)0xFF && message->payload[1] == (byte)0x1)) {
        action = &Parser::_run;
        sprintf(filename, "./%d_SESSION.txt", message->timestamp);
        file.open(filename);
        file << message->timestamp << " " << message->data << endl;

        stats.init(message->timestamp);
        stats.report(0xA0, message->data, message->timestamp);
    }

    log(message, "idle");
}

void Parser::_run(CanMessage *message) {

    string data(message->data, message->length);
    file << message->timestamp << " " << data << endl;

    stats.report(message->id, message->data, message->timestamp);
    
    // Check the first part of the data (0A0) before checking the entire string
    //if (strncmp(message->data, "0A0", 3) == 0 && strncmp(&message->data[4], "66FF", 4) == 0) {
    if (message->id == 0xA0 && message->payload[0] == (byte)0x66 && message->payload[1] == (byte)0xFF) {
        file.close();
        stats.build();
        action = &Parser::_idle;
        return;
    }

    log(message, "run");
}

void Parser::join() {
    thr->join();
}

void parseBytes(CanMessage *message) {

    string id (message->data, 3);
    message->id = stoi(id, 0, 16);

    for (int i = 0; i < message->length / 2 - 2 && message->data[4+i*2] != '\0'; i++) {
        string payload(&message->data[4+i*2], 2);
        message->payload[i] = (byte) stoi(payload, 0, 16);
    }
}

void Parser::setReturnOnEmpty() {
    queue->stopDequeue();
    returnOnEmptyQueue = [](CanMessage *message){ 
            if (message->id == 0) exit(0); 
        };
}