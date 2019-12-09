//
// Created by Carlos Hidalgo on 2019-11-21.
//

#include <stdlib.h>
#include <stdio.h>
#include "Graph.h"
#include "List.h"
#include "Queue.h"
#include "Set.h"
#include "PriorityQueue.h"

struct strGraph {
    List adjacencyList;
    CompareFunc cF;
    DestroyFunc dF;
    PrintFunc pF;
    int debug;
    int DFS_time;
};

struct strVertex {
    Type n;
    char color;
    double dist;
    int Tdiscover;
    int Ttermination;
    struct strVertex *parent;
};
typedef struct strVertex Vertex;

struct strPair {
    Type v;
    double weight;
};
typedef struct strPair Pair;

void graph_print(Graph g);
int *graph_findEdge(Graph g, Type u, Type v);

Graph graph_create(PrintFunc printer, DestroyFunc destructor, CompareFunc comp) {
    Graph g = calloc(1, sizeof(struct strGraph));
    g->dF = destructor;
    g->pF = printer;
    g->cF = comp;
    g->debug = -1;
    return g;
}

void graph_destroy(Graph g) {
    if (!g) return;
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        list_destroy(list_data(it));
    }
    list_destroy(g->adjacencyList);
    free(g);
}

void graph_addVertex(Graph g, Type u) {
    if (!g) return;
    if (!g->adjacencyList) {
        g->adjacencyList = list_create(g->dF);
    }
    List tL = list_create(g->dF);
    Vertex *uV = calloc(1, sizeof(struct strVertex));
    uV->n = u;
    list_add(tL, uV);
    list_add(g->adjacencyList, tL);
    if (g->debug == 0) graph_print(g);
}

void graph_deleteVertex(Graph g, Type v) {
    if (!g) return;
    // Borrar todos los Edges que lleven al Vertex que se va a eliminar
    Iterator dE = list_begin(g->adjacencyList);
    while (list_hasNext(dE)) {
        dE = (Iterator) list_next(dE);
        Vertex *tV = list_get(list_data(dE), 0);
        graph_deleteEdge(g, tV->n, v);
    }
    // Borrar el Vertex
    int f_v = 1, idx_v = -1;
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        ++idx_v;
        // Compara el elemento 0 de la lista en el iterador con v
        Vertex *tV = list_get(list_data(it), 0);
        f_v = g->cF(tV->n, v);
        if (f_v == 0) {
            list_destroy(list_data(it));
            list_remove(g->adjacencyList, idx_v);
            if (g->debug == 0) printf("SI se borro vertex\n");
            if (g->debug == 0) graph_print(g);
            return;
        }
    }
    if (g->debug == 0) printf("NO se borro vertex\n");
    if (g->debug == 0) graph_print(g);
}

void graph_addEdge(Graph g, Type u, Type v, double weight) {
    if (!g) return;
    // f_x son banderas found para los nodos u y v
    int f_u = 1, f_v = 1, idx_u = -1;
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        // Si no ha encontrado a v, compara el elemento 0
        // de la lista en el iterador con v
        Vertex *tV = list_get(list_data(it), 0);
        if (f_v != 0) f_v = g->cF(tV->n, v);
        // Compara el elemento 0 de la lista en el iterador con u
        if (f_u != 0) {
            idx_u++;
            f_u = g->cF(tV->n, u);
        }
        if (f_u == 0 && f_v == 0) break;
    }
    if (f_u != 0 || f_v != 0) {
        if (g->debug == 0) printf("NO se agrego edge\n");
        if (g->debug == 0) graph_print(g);
        return;
    }
    Pair *data = calloc(1, sizeof(struct strPair));
    data->v = v;
    data->weight = weight;
    Iterator Addit = list_begin(g->adjacencyList);
    while (list_hasNext(Addit)) {
        Addit = list_next(Addit);
        Vertex *tV = list_get(list_data(Addit), 0);
        if (g->cF(tV->n, u) == 0) {
            list_add(list_data(Addit), data);
            break;
        }
    }
    if (g->debug == 0) printf("SI se agrego edge\n");
    if (g->debug == 0) graph_print(g);
}

void graph_deleteEdge(Graph g, Type u, Type v) {
    if (!g) return;
    // f_u es badera para found el nodo u
    int f_u = 1;
    List tL = NULL;
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        // Si no ha encontrado a u, compara el elemento 0
        // de la lista en el iterador con u
        Vertex *tV = list_get(list_data(it), 0);
        f_u = g->cF(tV->n, u);
        if (f_u == 0) {
            tL = list_data(it);
            break;
        }
    }
    if (f_u != 0) {
        printf("NO se elimino edge\n");
        return;
    }
    it = list_begin(tL);
    it = (Iterator) list_next(it);
    // f_u es badera para found el nodo v
    int f_v = 1, p = 0;
    Pair *tP = NULL;
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        p++;
        // Compara el elemento en el iterador con v
        tP = (Pair *) list_data(it);
        f_v = g->cF(tP->v, v);
        if (f_v == 0) {
            list_remove(tL, p);
            if (g->debug == 0) printf("Si se elimino edge\n");
            if (g->debug == 0) graph_print(g);
            break;
        }
    }
}

void graph_mode(Graph g, int m) {
    g->debug = m;
}

void graph_print(Graph g) {
    if (!g || list_size(g->adjacencyList) <= 0) return;
    printf("Tamanio de adjList: %d\n", list_size(g->adjacencyList));
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        List nl = list_data(it);
        Vertex *V0 = list_get(nl, 0);
        printf("\nNodo: ");
        g->pF(V0->n);
        printf("\n");
        // -Color: %c
        printf("-Distancia: %f\n-Ti: %d\n-Tf: %d\n-Padre: ", /*V0->color,*/ V0->dist, V0->Tdiscover,
               V0->Tdiscover);
        if (V0->parent) g->pF(V0->parent->n);
        else printf("NULO");
        printf("\n");
        if (list_size(nl) == 1) {
            continue;
        }
        Iterator itn = list_begin(nl);
        itn = (Iterator) list_next(itn);
        while (list_hasNext(itn)) {
            itn = (Iterator) list_next(itn);
            printf(" -> {");
            Pair *datos = list_data(itn);
            g->pF(datos->v);
            printf(", %f }", datos->weight);
        }
        printf("\n");
    }
}



void BFS(Graph g, Type start) {
    // Setup
    Queue q = queue_create(NULL);
    Iterator setit = list_begin(g->adjacencyList);
    while (list_hasNext(setit)) {
        setit = (Iterator) list_next(setit);
        Vertex *tV = list_get(list_data(setit), 0);
        tV->color = 'B';
        tV->dist = -42069;
        tV->parent = NULL;
    }

    // Setup start
    Iterator dE = list_begin(g->adjacencyList);
    while (list_hasNext(dE)) {
        dE = (Iterator) list_next(dE);
        Vertex *sV = list_get(list_data(dE), 0);
        if (g->cF(sV->n, start) == 0) {
            sV->color = 'G';
            sV->dist = 0;
            sV->parent = NULL;
            queue_offer(q, sV);
            break;
        }
    }

    // Recorrido
    while (!queue_isEmpty(q)) {
        Vertex *uV = queue_poll(q);

        // Encontrar Lista de adyacencia de uV
        Iterator Ait = list_begin(g->adjacencyList);
        while (list_hasNext(Ait)) {
            Ait = (Iterator) list_next(Ait);
            Vertex *sV = list_get(list_data(Ait), 0);
            if (g->cF(sV->n, uV->n) == 0) {

                // Ya que lo encontró iterar por los hijos
                Iterator it = list_begin(list_data(Ait));
                it = (Iterator) list_next(it);
                Pair *tP = NULL;
                while (list_hasNext(it)) {
                    it = (Iterator) list_next(it);
                    tP = (Pair *) list_data(it);
                    Type son = tP->v;
                    Iterator sit = list_begin(g->adjacencyList);
                    while (list_hasNext(sit)) {
                        sit = (Iterator) list_next(sit);
                        Vertex *tV = list_get(list_data(sit), 0);

                        // Se procesan los hijos
                        if (g->cF(tV->n, son) == 0 && tV->color == 'B') {
                            tV->color = 'G';
                            tV->dist = 1 + uV->dist;
                            tV->parent = uV;
                            queue_offer(q, tV);
                        }
                    }
                }
            }
        }
        uV->color = 'N';
    }
    graph_print(g);
}

void DFS_recursive(Graph g, List u) {
    Vertex *uVtx = list_get(u, 0);
    uVtx->Tdiscover = ++g->DFS_time;
    uVtx->color = 'G';
    if (list_size(u) > 1) {
        Iterator it = list_begin(u);
        it = (Iterator) list_next(it);
        Pair *tP = NULL;
        while (list_hasNext(it)) {
            it = (Iterator) list_next(it);
            tP = (Pair *) list_data(it);
            Type son = tP->v;

            Iterator dE = list_begin(g->adjacencyList);
            while (list_hasNext(dE)) {
                dE = (Iterator) list_next(dE);
                Vertex *tV = list_get(list_data(dE), 0);
                if (g->cF(tV->n, son) == 0 && tV->color == 'B')
                    DFS_recursive(g, list_data(dE));
            }
        }
    }
    uVtx->color = 'N';
    uVtx->Ttermination = ++g->DFS_time;
}

void DFS(Graph g) {
    // Setup
    Iterator setit = list_begin(g->adjacencyList);
    while (list_hasNext(setit)) {
        setit = (Iterator) list_next(setit);
        Vertex *tV = list_get(list_data(setit), 0);
        tV->color = 'B';
        tV->parent = NULL;
    }
    //Tiempo
    g->DFS_time = 0;
    // Recursion
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        List Node = list_data(it);
        Vertex *u = list_get(list_data(it), 0);
        if (u->color == 'B') DFS_recursive(g, Node);
    }
    graph_print(g);
}


void dijkstra(Graph g, Type start) {
    //Cola de prioridad minima
    PriorityQueue Q = priorityqueue_create(g->dF, g->cF, 30);
    //Vertice de inicio
    Vertex *s = NULL;
    //setup start
    Iterator dE = list_begin(g->adjacencyList);
    //Por cada vertice
    while (list_hasNext(dE)) {
        dE = (Iterator) list_next(dE);
        Vertex *sV = list_get(list_data(dE), 0);
        //La distancia es infinita
        sV->dist = 42069;
        //El padre es nulo
        sV->parent = NULL;
        //Si es el vertice de inicio la distancia es 0
        if (g->cF(sV->n, start) == 0) {
            sV->dist = 0;
            s = sV;
        }
    }
    //Conjunto vacio
    Set S = set_create(g->cF, g->pF, g->dF);
    //Insertar s en la cola de prioridad Q
    priorityqueue_offer(Q, s);

    //Mientras Q no este vacio
    while (priorityqueue_size(Q) > 0){
        //Tomar el nodo minimo de Q: u
        Vertex * u = priorityqueue_poll(Q);
        //Agregar u al conjunto S
        set_add(S, u);
        //Por cada vertice v en la lista de adyacencia de u y no contenido en S
        Iterator adj = list_begin(g->adjacencyList);
        while (list_hasNext(adj) && !set_contains(S, list_get(list_data(list_next(adj)), 0))) {
            adj = (Iterator) list_next(adj);
            Vertex *sV = list_get(list_data(adj), 0);

            //Si la distancia de v es mayor que la suma de la distancia de u y el peso de la arista:
            Iterator findit = list_begin(g->adjacencyList);
            Iterator findv = NULL;
            while (list_hasNext(findit)) {
                findit = (Iterator) list_next(findit);
                Vertex *tV = list_get(list_data(findit), 0);
                if (g->cF(u->n, tV->n) == 0 ) {
                    findv = (Iterator)list_begin(list_data(findit));
                    break;
                }
            }
            Pair * tPair = NULL;
            while (list_hasNext(findv)) {
                findv = (Iterator) list_next(findv);
                tPair = list_data(findv);
                if (g->cF(sV->n, tPair->v) == 0 ) break;
            }

            if (sV->dist > (u->dist + tPair->weight)) {
                sV->dist = u->dist + tPair->weight;
                sV->parent = u;
                set_add(S, sV);
                priorityqueue_offer(Q, sV);
            }
            graph_print(g);
        }
    }
}

int* graph_findEdge(Graph g, Type u, Type v) {
    int* res = calloc(2, sizeof(int));
    res[0] = res[1] = -1;
    if (!g) return res;
    // f_u es badera para found el nodo u
    int f_u = 1;
    List tL = NULL;
    //Primer indice
    int ind1 = -1;
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        // Si no ha encontrado a u, compara el elemento 0
        // de la lista en el iterador con u
        Vertex *tV = list_get(list_data(it), 0);
        f_u = g->cF(tV->n, u);
        if (f_u == 0) {
            tL = list_data(it);
            break;
        }
        ind1++;
    }
    res[0] = ind1;
    if (res[0] == -1) return res;
    it = list_begin(tL);
    it = (Iterator) list_next(it);
    // f_u es badera para found el nodo v
    int f_v = 1, p = 0;
    Pair *tP = NULL;
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        p++;
        // Compara el elemento en el iterador con v
        tP = (Pair *) list_data(it);
        f_v = g->cF(tP->v, v);
        if (f_v == 0) {
            res[1] = p;
            return res;
        }
    }
}