#pragma once

#include <ctime>

extern "C" {
    #include "fake_receiver.h"
}

using namespace std;

typedef struct CanMessage {
    int length;
    time_t timestamp;
    char data[MAX_CAN_MESSAGE_SIZE];
    uint16_t id;
    byte payload[8];
} CanMessage;