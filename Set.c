//
// Created by Carlos Hidalgo on 2019-11-13.
//
#include "Set.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct strNode * Node;
struct strNode {
    Type data;
    struct strNode *left, *right, *parent;
};

struct strSet {
    struct strNode *root;	//desde la raíz llegamos a cualquier elemento
    int size;
    CompareFunc compareFunc;
    PrintFunc printFunc;
    DestroyFunc destroyFunc;
};

void destroy(Node pNode, DestroyFunc pFunction);
void removeBranch(Set s, Node n, bool isLeftChild);
void print(Node node, PrintFunc printFunc);
void addLink(Node parent, Node child, bool isLeftChild);

Set set_create(CompareFunc cf, PrintFunc pf, DestroyFunc df){//Listo
    Set s = malloc(sizeof(struct strSet));
    s->root = NULL;
    s->size = 0;
    s->compareFunc = cf;
    s->printFunc = pf;
    s->destroyFunc = df;
}

int set_size(Set s){
    return s->size;
}

bool set_add(Set s, Type t){
    if (set_contains(s, t)) return FALSE;
    Node n = malloc(sizeof(struct strNode));
    n->data = t;
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    if (!s->size) s->root = n;
    Node curr = s->root;

    while(set_size(s)){
        if (s->compareFunc(t,curr->data) > 0) {
            if(curr->right == NULL){
                addLink(curr, n, FALSE);
                break;
            } else {
                curr = curr->right;
                continue;
            }
        } else {
            if(curr->left == NULL){
                addLink(curr, n, TRUE);
                break;
            } else {
                curr = curr->left;
                continue;
            }
        }
    }

    s->size++;
    return TRUE;
}

bool set_contains(Set s, Type t){//Listo
    if (s->size == 0) return FALSE;
    Node n = s->root;
    int cmp = 0;
    while (n != NULL) {
        cmp = s->compareFunc(t, n->data);
        if (cmp == 0) return TRUE;
        if (cmp > 0) n = n->right;
        else n = n->left;
    }
    return FALSE;
}

void set_destroy(Set s){//Listo
    destroy(s->root, s->destroyFunc);
}

bool set_remove(Set s, Type t){//Listo
    if (!set_contains(s, t)) return FALSE;
    Node n = s->root;
    int cmp;
    bool isleft = FALSE;
    while (n != NULL) {
        cmp = s->compareFunc(t, n->data);
        if (cmp == 0) break;
        if (cmp > 0){
            n = n->right;
            isleft = FALSE;
        }
        else{
            n = n->left;
            isleft = TRUE;
        }
    }
    removeBranch(s, n, isleft);
    s->size--;
    return TRUE;
}

void set_print(Set s){//Listo
    print(s->root, s->printFunc);
    printf("\n");
}

void addLink(Node parent, Node child, bool isLeftChild){//Listo
    if (isLeftChild) parent->left = child;
    else parent->right = child;
    if (child) child->parent = parent;
}

void destroy(Node pNode, DestroyFunc pFunction){//Listo
    if (!pNode) return;
    if (pNode->right) destroy(pNode->right, pFunction);
    if (pNode->left) destroy(pNode->left, pFunction);
    if (pFunction) pFunction(pNode->data);
    free(pNode);
}

void print(Node node, PrintFunc printFunc){//Listo
    if (!node) return;
    printf("{");
    printFunc(node->data);
    print(node->left, printFunc);
    print(node->right, printFunc);
    printf("}");
}

void removeBranch(Set s, Node n, bool isLeftChild){//Listo
    //Si no tiene hijos, solo destruir el nodo
    if (!n->left && !n->right){
        if (n == s->root) s->root = NULL;
        else {
            if (isLeftChild) n->parent->left = NULL;
            else n->parent->right = NULL;
        }
        if (s->destroyFunc) s->destroyFunc(n);
        free(n);
    }
    //Si tiene hijo a la izquierda
    else if (!n->right){
        if (n == s->root) s->root = n->left;
        else {
            addLink(n->parent, n->left, isLeftChild);
        }
        if (s->destroyFunc) s->destroyFunc(n);
        free(n);
    }
    //Si tiene hijo a la derecha
    else if (!n->left){
        if (n == s->root) s->root = n->right;
        else {
            addLink(n->parent, n->right, isLeftChild);
        }
        if (s->destroyFunc) s->destroyFunc(n);
        free(n);
    }
    //Si tiene 2 hijos
    else {
        Node m = n->left;
        while (m->right) {
            if (m->right) m = m->right;
            else break;
        }
        n->data = m->data;
        if (m->parent == n || m->parent == s->root) addLink(m->parent, m->left, TRUE);
        else addLink(m->parent, m->left, FALSE);
        if (s->destroyFunc) s->destroyFunc(m);
        free(m);
    }
}
