//
// Created by rln on 2019-12-06.
//

#include <stdlib.h>
#include "List.h"
#include "Queue.h"


struct QstrNode
{
    Type data;
    struct QstrNode * next;
};

typedef struct QstrNode * QNode;

struct strQueue
{
    struct QstrNode * first, * last;
    int size;
    void (*destructor)(void *);
};


Queue queue_create(void (*destructor)(void *))
{
    Queue q = (Queue) malloc(sizeof(struct strQueue));

    q->size = 0;
    q->first = q->last = NULL;
    q->destructor = destructor;

    return q;
}

int queue_size(Queue q)
{
    if(q == NULL)
        return -1;

    return q->size;
}

bool queue_isEmpty(Queue q)
{
    if(q == NULL)
        return TRUE;

    return q->size == 0;
}


void queue_offer(Queue q, Type t)
{
    if(q == NULL)
        return;
     QNode n = (Node) malloc(sizeof(struct QstrNode*));
    n->data = t;
    n->next = NULL;

    if(queue_isEmpty(q))
        q->first = n;
    else
        q->last->next = n;

    q->last = n;
    q->size++;
}

Type queue_peek(Queue q)
{
    if(q == NULL || q->size == 0)
        return NULL;

    return q->first->data;
}

Type queue_poll(Queue q)
{
    if(q == NULL || q->size == 0)
        return NULL;

    QNode aux = q->first;
    Type data = q->first->data;

    q->first = q->first->next;
    q->size--;

    if(q->size == 0)
        q->last = NULL;

    free(aux);

    return data;
}

void queue_destroy(Queue q)
{
    if(q == NULL)
        return;

    QNode aux1, aux2;

    aux1 = q->first;

    while(aux1 != NULL)
    {
        aux2 = aux1->next;

        if(q->destructor != NULL)
            q->destructor(aux1->data);

        free(aux1);
        aux1 = aux2;
    }

    free(q);
}