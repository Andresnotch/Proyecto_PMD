//
// Created by Carlos Hidalgo on 2019-11-21.
//

#include <stdlib.h>
#include "Graph.h"
#include "List.h"

struct strGraph {
    List adjacencyList;
    CompareFunc cF;
    DestroyFunc dF;
    PrintFunc pF;
};

Graph graph_create(PrintFunc printer, DestroyFunc destructor, CompareFunc comp) {
    Graph g = calloc(1,sizeof(struct strGraph));
    g->dF = destructor;
    g->pF = printer;
    g->cF = comp;
    return g;
}

void graph_destroy(Graph g) {
    if(!g) return NULL;
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        list_destroy(list_data(it));
    }
    list_end(it);
    list_destroy(g->adjacencyList);
    free(g);
}

void graph_addVertex(Graph g, Type u) {
    if(!g) return NULL;
    if (!g->adjacencyList) {
        g->adjacencyList = list_create(g->dF);
    }
    List tL = list_create(g->dF);
    list_add(tL,u);
    list_add(g->adjacencyList, tL);
}

void graph_deleteVertex(Graph g, Type v) {

}

void graph_addEdge(Graph g, Type u, Type v, double weight) {
    if(!g) return NULL;
    int f_u = 1, f_v = 1, n_u=-1,n_v=-1;
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        if(!f_v) f_v = g->cF(list_data(it),v);
        if(!f_u) {
            f_u = g->cF(list_data(it),u);
            n_u++;
        }
    }
    list_end(it);
    if(!f_u || !f_v) return;
    List tL = list_get(g->adjacencyList,n_u);
    list_add(tL,v);
}

void graph_deleteEdge(Graph g, Type u, Type v) {

}


void BFS(Graph g, Type start) {

}

void DFS(Graph g) {

}

void dijkstra(Graph g, Type start) {

}
