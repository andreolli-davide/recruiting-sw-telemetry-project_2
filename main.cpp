#include <stdio.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include "queue.hpp"
#include "receiver.hpp"

using namespace std;

#define DUMP_LOG_FILENAME "candump.log"

MessageQueue queue;
Receiver r(&queue);

int main(void){

    // Start receiver thread
    r.start();
    r.thr->join();
    return 0;
}