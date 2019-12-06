//
// Created by rln on 2019-11-28.
//

#ifndef PROYECTO_LIST_H
#define PROYECTO_LIST_H

#include "bool.h"

typedef struct strList *List;
typedef struct strNode *Node;
typedef Node Iterator;


List list_create(void (*destructor)(Type));

void list_destroy(List);

int list_size(List);

void list_add(List, Type);

Type list_get(List, int p);

void list_set(List, Type, int p);

Type list_remove(List, int p);

Iterator list_begin(List);

Iterator list_end(List);

bool list_hasNext(Iterator);

bool list_hasPrior(Iterator);

Iterator list_next(Iterator);

Iterator list_prior(Iterator);

Type list_data(Iterator);

#endif //PROYECTO_LIST_H
