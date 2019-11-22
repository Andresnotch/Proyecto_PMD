//
// Created by Carlos Hidalgo on 2019-11-21.
//

#include <stdlib.h>
#include "../../../../../../Desktop/Proyecto/Graph.h"

struct strGraph {
    Type **adjacencyList;
    CompareFunc compareFunc;
    DestroyFunc destroyFunc;
    PrintFunc printFunc;
};

Graph graph_create(PrintFunc printer, DestroyFunc destructor){
    Graph g = malloc(sizeof(struct strGraph));
    g->destroyFunc = destructor;
    g->printFunc = printer;
    return g;
}

void graph_destroy(Graph g){

}

void graph_addVertex(Graph g, Type u){

}

void graph_deleteVertex(Graph g, Type v){

}

void graph_addEdge(Graph g, Type u, Type v, double weight){

}

void graph_deleteEdge(Graph g, Type u, Type v){

}


void BFS(Graph g, Type start){

}

void DFS(Graph g){

}

void dijkstra(Graph g, Type start){

}
