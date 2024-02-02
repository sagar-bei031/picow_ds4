// shared.c
#include "shared.h"

// Define the shared queue
queue_t shared_queue;

// Implementation of the initialization function
void shared_init() {
    queue_init(&shared_queue, sizeof(Message), MAX_QUEUE_SIZE);
}
