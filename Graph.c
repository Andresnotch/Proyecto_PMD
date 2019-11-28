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
    if(!g) return NULL;
    // Bandera found para v
    int f_v = 1;
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        // Compara el elemento 0 de la lista en el iterador con v
        f_v = g->cF(list_get(list_data(it),0),v);
        if (f_v) {
            list_destroy(list_data(it));
            break;
        }
    }
}

void graph_addEdge(Graph g, Type u, Type v, double weight) {
    if(!g) return NULL;
    // f_x son banderas found para los nodos u y v
    int f_u = 1, f_v = 1;
    List tL = NULL;
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        // Si no ha encontrado a v, compara el elemento 0
        // de la lista en el iterador con v
        if(!f_v) f_v = g->cF(list_get(list_data(it),0),v);
        // Compara el elemento 0 de la lista en el iterador con u
        f_u = g->cF(list_get(list_data(it),0),u);
        if (f_u) {
            tL = list_data(it);
            if(f_v) break;
        }
    }
    list_end(it);
    if(!f_u || !f_v) return;
    list_add(tL,v);
}

void graph_deleteEdge(Graph g, Type u, Type v) {
    if(!g) return NULL;
    // f_u es badera para found el nodo u
    int f_u = 1;
    List tL = NULL;
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        // Compara el elemento 0 de la lista en el iterador con u
        f_u = g->cF(list_get(list_data(it),0),u);
        if (f_u) {
            tL = list_data(it);
            break;
        }
    }
    list_end(it);
    if(!f_u) return;
    it = list_begin(tL);
    // f_u es badera para found el nodo v
    int f_v = 1;
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        // Compara el elemento en el iterador con v
        f_v = g->cF(list_data(it),v);
        if (f_v) {
            list_destroy(list_data(it));
            break;
        }
    }
}


void BFS(Graph g, Type start) {

}

void DFS(Graph g) {

}

void dijkstra(Graph g, Type start) {

}
