//
// Created by Carlos Hidalgo on 2019-11-17.
//

#ifndef TAREAS_PRIORITYQUEUE_H
#define TAREAS_PRIORITYQUEUE_H

#include "bool.h"

typedef struct strPriorityQueue *PriorityQueue;

typedef int (*CompareFunc)(Type, Type);

typedef void (*DestroyFunc)(Type);

PriorityQueue priorityqueue_create(DestroyFunc, CompareFunc, int capacity);

void priorityqueue_destroy(PriorityQueue);

int priorityqueue_size(PriorityQueue);

void priorityqueue_offer(PriorityQueue, Type);

Type priorityqueue_peek(PriorityQueue);

Type priorityqueue_poll(PriorityQueue);

#endif //TAREAS_PRIORITYQUEUE_H
