//
// Created by Carlos Hidalgo on 2019-11-21.
//

#include <stdlib.h>
#include <stdio.h>
#include "Graph.h"
#include "List.h"

struct strGraph {
    List adjacencyList;
    CompareFunc cF;
    DestroyFunc dF;
    PrintFunc pF;
    int debug;
};

struct strPair {
    Type v;
    double weight;
};
typedef struct strPair Pair;

void graph_print(Graph g);

Graph graph_create(PrintFunc printer, DestroyFunc destructor, CompareFunc comp) {
    Graph g = calloc(1,sizeof(struct strGraph));
    g->dF = destructor;
    g->pF = printer;
    g->cF = comp;
    g->debug = -1;
    return g;
}

void graph_destroy(Graph g) {
    if(!g) return;
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        list_destroy(list_data(it));
    }
    list_destroy(g->adjacencyList);
    free(g);
}

void graph_addVertex(Graph g, Type u) {
    if(!g) return;
    if (!g->adjacencyList) {
        g->adjacencyList = list_create(g->dF);
    }
    List tL = list_create(g->dF);
    list_add(tL,u);
    list_add(g->adjacencyList, tL);
    if (g->debug == 0) graph_print(g);
}

void graph_deleteVertex(Graph g, Type v) {
    if(!g) return;
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
    if(!g) return;
    // f_x son banderas found para los nodos u y v
    int f_u = 1, f_v = 1;
    List tL = NULL;
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        // Si no ha encontrado a v, compara el elemento 0
        // de la lista en el iterador con v
        if(f_v != 0) f_v = g->cF(list_get(list_data(it), 0), v);
        // Compara el elemento 0 de la lista en el iterador con u
        if(f_u != 0) f_u = g->cF(list_get(list_data(it),0),u);
        if (f_u == 0) {
            tL = list_data(it);
            if(f_v == 0) break;
        }
    }
    Pair data;
    data.v = v;
    data.weight = weight;
    if(f_u != 0 || f_v != 0) {
        if(g->debug == 0) printf("NO se agrego edge\n");
        if(g->debug == 0) graph_print(g);
        return;
    }
    list_add(tL,&data);
    if(g->debug == 0) printf("SI se agrego edge\n");
    if(g->debug == 0) graph_print(g);
}

void graph_deleteEdge(Graph g, Type u, Type v) {
    if(!g) return;
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

void graph_mode(Graph g, int m) {
    g->debug = m;
}

void graph_print(Graph g) {
    if(!g || list_size(g->adjacencyList) <= 0) return;
    printf("Tamanio de adjList: %d\n",list_size(g->adjacencyList));
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        List nl = list_data(it);
        g->pF(list_get(nl,0));
        if(list_size(nl) == 1){
            printf("\n");
            continue;
        }
        Iterator itn = list_begin(nl);
        while (list_hasNext(it)) {
            for (int i = 0; i < 2; ++i)
                itn = (Iterator) list_next(it);
            printf(" -> {");
            Pair* datos =  list_data(itn);
            g->pF(datos->v);
            printf(", %f }",datos->weight);
        }
        printf("\n");
    }
}

void BFS(Graph g, Type start) {

}

void DFS(Graph g) {

}

void dijkstra(Graph g, Type start) {
    // Inicializar todas las distancias en D con un valor infinito relativo, ya que son desconocidas al principio,
    // exceptuando la de x, que se debe colocar en 0, debido a que la distancia de x a x sería 0.

    // Sea a = x (Se toma a como nodo actual.)

    // Se recorren todos los nodos adyacentes de a, excepto los nodos marcados. Se les llamará nodos no marcados vi.

    // Para el nodo actual, se calcula la distancia tentativa desde dicho nodo hasta sus vecinos con la siguiente
    // fórmula: dt(vi) = Da + d(a,vi). Es decir, la distancia tentativa del nodo ‘vi’ es la distancia que actualmente
    // tiene el nodo en el vector D más la distancia desde dicho nodo ‘a’ (el actual) hasta el nodo vi. Si la distancia
    // tentativa es menor que la distancia almacenada en el vector, entonces se actualiza el vector con esta distancia
    // tentativa. Es decir, si dt(vi) < Dvi → Dvi = dt(vi)

    // Se marca como completo el nodo a.

     // Se toma como próximo nodo actual el de menor valor en D (puede hacerse almacenando los valores en una cola de
     // prioridad) y se regresa al paso 3, mientras existan nodos no marcados.



}
