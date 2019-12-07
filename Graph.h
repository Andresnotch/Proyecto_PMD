//
// Created by Carlos Hidalgo on 2019-11-21.
//

#ifndef PROYECTO_GRAPH_H
#define PROYECTO_GRAPH_H

typedef void *Type;

typedef int (*CompareFunc)(Type, Type);

typedef void (*PrintFunc)(Type);

typedef void (*DestroyFunc)(Type);

typedef struct strGraph *Graph;

Graph graph_create(PrintFunc printer, DestroyFunc destructor, CompareFunc comp);

void graph_destroy(Graph g);

void graph_addVertex(Graph g, Type u);

void graph_deleteVertex(Graph g, Type v);

void graph_addEdge(Graph g, Type u, Type v, double weight);

void graph_deleteEdge(Graph g, Type u, Type v);

void graph_mode(Graph g, int m);

void BFS(Graph g, Type start);

void DFS(Graph g, Type start);

void dijkstra(Graph g, Type start);

#endif //PROYECTO_GRAPH_H
