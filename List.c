//
// Created by rln on 2019-11-25.
//

#include "List.h"

#include <stdlib.h>

struct strNode {
    Type data;
    struct strNode *next, *prior;
};

struct strList {
    struct strNode *first, *last, *preFirst, *postLast;
    int size;

    void (*destructor)(void *);
};


List list_create(void (*destructor)(Type)) {
    List l = (List) malloc(sizeof(struct strList));
    l->size = 0;
    l->first = l->last = l->postLast = l->preFirst = NULL;
    l->destructor = destructor;

    return l;
}

void list_destroy(List l) {
    if (l == NULL) return;

    Node aux1, aux2;

    aux1 = l->first;

    while (aux1 != NULL) {
        aux2 = aux1->next;

        if (l->destructor != NULL)
            l->destructor(aux1->data);

        free(aux1);
        aux1 = aux2;
    }
    free(l->postLast);
    free(l->preFirst);
    free(l);
}

int list_size(List l) {
    if (l == NULL) return -1;
    return l->size;
}

void list_add(List l, Type t) {
    if (l == NULL) return;

    Node n = (Node) malloc(sizeof(struct strNode));
    n->data = t;
    n->prior = NULL;
    n->next = NULL;

    if (l->size == 0) {
        l->preFirst = (Node) malloc(sizeof(struct strNode));
        l->postLast = (Node) malloc(sizeof(struct strNode));
        l->last = l->first = n;
        l->preFirst->next = l->postLast->prior = n;
    } else {
        n->prior = l->last;
        l->last->next = n;
    }

    l->postLast->prior = l->last = n;
    l->size++;
}

Type list_get(List l, int p) {
    Node current = NULL;
    if (p < 0 || p >= l->size) return current;
    else if (p < l->size / 2) {
        current = l->first;
        for (int i = 0; i < p; ++i) current = current->next;

    } else if (p <= l->size) {
        current = l->last;
        for (int i = 0; i < p; ++i) current = current->prior;
    }
    return current->data;
}

void list_set(List l, Type t, int p) {
    Node current = (Node) malloc(sizeof(struct strNode));
    current = NULL;
    if (p < 0 || p >= l->size) return;
    else if (p < l->size / 2) {
        current = l->first;
        for (int i = 0; i < p; ++i) current = current->next;

    } else if (p <= l->size) {
        current = l->last;
        for (int i = 0; i < p; ++i) current = current->prior;
    }
    current->data = t;
}

Type list_remove(List l, int p) {
    Node current = NULL;
    Type *toReturn;
    if (p == 0 && l->size == 1) {
        toReturn = l->first->data;
        free(l->first);
        free(l->preFirst);
        free(l->postLast);
        l->first = l->last = l->postLast = l->preFirst = NULL;
        l->size--;
        return toReturn;
    }
    if (p < 0 || p >= l->size) return current;
    else if (p < l->size / 2) {
        current = l->first;
        for (int i = 0; i < p; ++i) current = current->next;

    } else if (p <= l->size) {
        current = l->last;
        for (int i = 0; i < p; ++i) current = current->prior;
    }

    if (current->next) current->next->prior = current->prior;
    if (current->prior) current->prior->next = current->next;
    toReturn = current->data;
    free(current);
    l->size--;

    return toReturn;
}

Iterator list_begin(List l) {
    Iterator i = l->preFirst;
    return i;

}

Iterator list_end(List l) {
    Iterator i = l->postLast;
    return i;
}

bool list_hasNext(Iterator i) {
    if (i && i->next) return TRUE;
    return FALSE;
}

bool list_hasPrior(Iterator i) {
    if (i && i->prior) return TRUE;
    return FALSE;
}

Iterator list_next(Iterator i) {
    if (list_hasNext(i)) i = i->next;
}

Iterator list_prior(Iterator i) {
    if (list_hasPrior(i)) i = i->prior;
}

Type list_data(Iterator i) {
    return i->data;
}
