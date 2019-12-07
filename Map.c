//
// Created by rln on 2019-12-06.
//

#include <stdio.h>
#include <stdlib.h>
#include "Map.h"
#include "List.h"
#include "bool.h"

struct strMap {
    int size;
    int m;
    List *table;
    HashFunc hashFunc;
    EqualsFunc equalsFunc;
    DestroyFunc kDestructor;
    DestroyFunc vDestructor;
};

struct strpair {
    Type key;
    Type value;
};
typedef struct strpair pair;

Map map_create(int m, HashFunc hF, EqualsFunc eF, DestroyFunc aDestructor, DestroyFunc bDestructor) {
    Map mapa = (Map) malloc(sizeof(struct strMap));
    mapa->size = 0;
    mapa->m = m;
    mapa->hashFunc = hF;
    mapa->equalsFunc = eF;
    mapa->kDestructor = aDestructor;
    mapa->vDestructor = bDestructor;
    mapa->table = (List *) calloc(m, sizeof(List));
    return mapa;
}

void map_destroy(Map m) {
    Iterator it = NULL;
    pair *temp = NULL;
    for (int i = 0; i < m->m; ++i) {
        if (!(m->table[i])) continue;
        it = list_begin(m->table[i]);
        while (list_hasNext(it)) {
            it = (Iterator) list_next(it);
            temp = (pair *) list_data(it);
            if (m->kDestructor) m->kDestructor(temp->key);
            if (m->vDestructor) m->vDestructor(temp->value);
        }
        list_destroy(m->table[i]);
    }
    free(m);
}

int map_size(Map m) {
    return m->size;
}

void map_put(Map m, Type k, Type v) {
    int hash = m->hashFunc(k, m->m);
    pair *datos = malloc(sizeof(struct strpair));
    datos->key = k;
    datos->value = v;
    List target = m->table[hash];
    if (!target) {
        target = list_create(NULL);
        m->table[hash] = target;
    }
    Iterator it = list_begin(m->table[hash]); int p = -1;
    bool found = FALSE;
    pair *temp = NULL;
    while (list_hasNext(it) && !found )  {
        it = (Iterator) list_next(it); p++;
        temp = list_data(it);
        found = m->equalsFunc(temp->key,v);
    }
    if(!found) list_add(m->table[hash], datos);
    else list_set(m->table[hash],datos,p);
    m->size++;
}

Type map_get(Map m, Type k) {
    int hash = m->hashFunc(k, m->m);
    int found = 0;
    Iterator it = list_begin(m->table[hash]);
    pair *temp = NULL;
    while (list_hasNext(it) && !found) {
        it = (Iterator) list_next(it);
        temp = (pair *) list_data(it);
        if (m->equalsFunc(temp->key, k))
            found = 1;
    }
    if (found) return (Type) temp->value;
    else return NULL;
}

Type map_remove(Map m, Type k) {
    int hash = m->hashFunc(k, m->m);
    int found = 0, p = -1;
    Iterator it = list_begin(m->table[hash]);
    pair *temp = NULL;
    while (list_hasNext(it) && !found) {
        p++;
        it = (Iterator) list_next(it);
        temp = (pair *) list_data(it);
        if (m->equalsFunc(temp->key, k))
            found = 1;
    }
    if (found) {
        list_remove(m->table[hash], p);
        m->size--;
        if (list_size(m->table[hash]) == 0) {
            list_destroy(m->table[hash]);
            m->table[hash] = NULL;
        }
        return (Type) temp->value;
    } else return NULL;
}

void map_print(Map m, PrintFunc Printk, PrintFunc Printv) {
    Iterator it = NULL;
    pair *temp = NULL;
    int count = 0;
    for (int i = 0; i < m->m; ++i) {
        if (!(m->table[i])) continue;
        printf("%i: ", i);
        it = list_begin(m->table[i]);
        count = 0;
        while (list_hasNext(it)) {
            it = (Iterator) list_next(it);
            temp = (pair *) list_data(it);
            if (count != 0) printf(", ");
            printf("< ");
            Printk(temp->key);
            printf(", ");
            Printv(temp->value);
            printf(" >");
            count++;
        }
        printf("\n");
    }
}