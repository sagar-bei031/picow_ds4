// shared.h
#ifndef _SHARED_H
#define _SHARED_H

#include "pico/util/queue.h"

#define MAX_QUEUE_SIZE 10

// Define a structure for your message
typedef struct {
    bool is_ps4_connected;
} Message;

// Define the shared queue
extern queue_t shared_queue;

// Function to initialize shared resources
void shared_init();

#endif