//
// Created by rln on 2019-12-06.
//

#ifndef PROYECTO_MAP_H
#define PROYECTO_MAP_H

//
// Created by rln on 2019-11-24.
//

#ifndef TAREA9_MAP_H
#define TAREA9_MAP_H

#include "bool.h"



typedef struct strMap * Map;


typedef int (*HashFunc) (Type, int);
typedef bool (*EqualsFunc) (Type, Type);
typedef void (*DestroyFunc) (Type);
typedef void (*PrintFunc) (Type);
typedef int (*CompareFunc)(Type, Type);


Map map_create(int m, HashFunc hF, EqualsFunc eF, DestroyFunc aDestructor, DestroyFunc bDestructor);
void map_destroy(Map);
int  map_size(Map);
void map_put(Map m, Type k, Type v);
Type map_get(Map m, Type k);
Type map_remove(Map m, Type k);

void map_print(Map m, PrintFunc Printk, PrintFunc Printv);


#endif //TAREA9_MAP_H

#endif //PROYECTO_MAP_H
