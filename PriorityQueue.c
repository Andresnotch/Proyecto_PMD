//
// Created by Carlos Hidalgo on 2019-11-17.
//
#include "PriorityQueue.h"
#include <stdio.h>
#include <stdlib.h>

struct strPriorityQueue {
    Type *root;
    int size;
    int capacity;
    CompareFunc compareFunc;
    DestroyFunc destroyFunc;
};

PriorityQueue priorityqueue_create(DestroyFunc dF, CompareFunc cF, int capacity) {
    PriorityQueue q = malloc(sizeof(struct strPriorityQueue));
    q->root = malloc(capacity * sizeof(Type));
    q->size = 0;
    q->capacity = capacity;
    q->destroyFunc = dF;
    q->compareFunc = cF;
    return q;
}

int parentIndex(int index);
int leftChildIndex(int index);
int rightChildIndex(int index);
bool isValidIndex(PriorityQueue q, int index);

void priorityqueue_destroy(PriorityQueue q) {
    for (int i = 0; i < q->size; ++i) {
        if (q->destroyFunc) q->destroyFunc(q->root[i]);
        free(q->root[i]);
    }
    free(q->root);
    free(q);
}

int priorityqueue_size(PriorityQueue q) {
    return q->size;
}

void priorityqueue_offer(PriorityQueue q, Type t) {
    if (q->capacity == q->size) {
        Type * newsize = realloc(q->root, 2 * q->capacity * sizeof(Type));
        if (newsize != NULL){
            q->root = newsize;
            q->capacity *= 2;
        }
    }
    q->root[q->size] = t;
    int t_index = q->size;
    q->size++;
    while (q->root[0] != q->root[t_index] && q->compareFunc(q->root[t_index], q->root[parentIndex(t_index)]) < 0) {
        Type temp = q->root[t_index];
        q->root[t_index] = q->root[parentIndex(t_index)];
        q->root[parentIndex(t_index)] = temp;
        t_index = parentIndex(t_index);
    }

    //Eliminar esto cuando termine de debuggear
    printf("[");
    for (int i = 0; i < q->size; ++i) {
        printf("%d, ", *(int*)(q->root[i]));
    }
    printf("]");
}

Type priorityqueue_peek(PriorityQueue q) {
    return q->root[0];
}

Type priorityqueue_poll(PriorityQueue q) {
    Type res = q->root[0];
    q->root[0] = q->root[--q->size];
    q->root[q->size] = NULL;
    int t_index = 0;
    while (t_index < q->size && (
            (isValidIndex(q, leftChildIndex(t_index)) && q->compareFunc(q->root[t_index], q->root[leftChildIndex(t_index)]) > 0) ||
            (isValidIndex(q, rightChildIndex(t_index)) && q->compareFunc(q->root[t_index], q->root[rightChildIndex(t_index)]) > 0)))
    {
        Type temp = q->root[t_index];
        int childindex = 0;
        if (isValidIndex(q, leftChildIndex(t_index)) && isValidIndex(q, rightChildIndex(t_index)) &&
            q->compareFunc(q->root[rightChildIndex(t_index)], q->root[leftChildIndex(t_index)]) < 0)
        {
            childindex = rightChildIndex(t_index);
        }
        else childindex = leftChildIndex(t_index);
        q->root[t_index] = q->root[childindex];
        q->root[childindex] = temp;
        t_index = childindex;
    }

    //Eliminar esto cuando termine de debuggear
    printf("\n[");
    for (int i = 0; i < q->size; ++i) {
        printf("%d, ", *(int*)(q->root[i]));
    }
    printf("]");

    return res;
}

int parentIndex(int index) {
    return (index - 1) / 2;
}

int leftChildIndex(int index) {
    return (2 * index) + 1;
}

int rightChildIndex(int index) {
    return (2 * index) + 2;
}

bool isValidIndex(PriorityQueue q, int index) {
    if (index >= 0 && index < priorityqueue_size(q)) return TRUE;
    return FALSE;
}
