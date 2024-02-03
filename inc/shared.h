// shared.h
#ifndef _SHARED_H
#define _SHARED_H

#include "pico/util/queue.h"

#define MAX_QUEUE_SIZE 10

typedef struct {
    bool is_ps4_connected;
} Message;

extern queue_t shared_queue;

void shared_init();

#endif