//
// Created by rln on 2019-12-06.
//

#ifndef PROYECTO_QUEUE_H
#define PROYECTO_QUEUE_H

#include "bool.h"

typedef struct strQueue * Queue;
typedef void (*destructor)(Type);

Queue queue_create(destructor d);
int queue_size(Queue q);
bool queue_isEmpty(Queue q);
void queue_offer(Queue q, Type t);
Type queue_peek(Queue q);
Type queue_poll(Queue q);
void queue_destroy(Queue q);

#endif //PROYECTO_QUEUE_H
