//
// Created by Carlos Hidalgo on 2019-11-13.
//

#ifndef TAREAS_SET_H
#define TAREAS_SET_H

#include "bool.h"

typedef struct strSet * Set;
typedef int (*CompareFunc) (Type, Type);
typedef void (*PrintFunc) (Type);
typedef void (*DestroyFunc) (Type);

Set set_create(CompareFunc cf, PrintFunc pf, DestroyFunc df);
int set_size(Set);
bool set_add(Set s, Type t);
bool set_contains(Set s, Type t);
void set_destroy(Set s);
bool set_remove(Set s, Type t);
void set_print(Set s);


#endif //TAREAS_SET_H
