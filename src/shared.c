#include "shared.h"

queue_t shared_queue;

void shared_init()
{
    queue_init(&shared_queue, sizeof(Message), MAX_QUEUE_SIZE);
}
