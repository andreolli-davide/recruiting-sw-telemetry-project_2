#include <stdio.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include "queue.hpp"
#include "receiver.hpp"
#include "parser.hpp"

using namespace std;

#define DUMP_LOG_FILENAME "candump.log"

MessageQueue queue;
Receiver r(&queue);
Parser p(&queue);

int main(void){

    // Start receiver and parser thread
    r.start();
    p.start();

    p.join();
    return 0;
}