#pragma once

#include <chrono>

extern "C" {
    #include "fake_receiver.h"
}

using namespace std;

typedef struct CanMessage {
    int length;
    chrono::time_point<chrono::system_clock> timestamp;
    char data[MAX_CAN_MESSAGE_SIZE];
} CanMessage;