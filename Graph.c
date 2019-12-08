//
// Created by Carlos Hidalgo on 2019-11-21.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Graph.h"
#include "List.h"
#include "Queue.h"

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
    Vertex* uV = calloc(1, sizeof(struct strVertex));
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
        graph_deleteEdge(g,tV->n,v);
    }
    // Borrar el Vertex
    int f_v = 1, idx_v = -1;
    Iterator it = list_begin(g->adjacencyList);
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        ++idx_v;
        // Compara el elemento 0 de la lista en el iterador con v
        Vertex * tV = list_get(list_data(it), 0);
        f_v = g->cF(tV->n, v);
        if (f_v == 0) {
            list_destroy(list_data(it));
            list_remove(g->adjacencyList,idx_v);
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
    int f_u = 1, f_v = 1, idx_u = -1 ;
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
        if (f_u == 0 && f_v == 0)  break;
    }
    if (f_u != 0 || f_v != 0) {
        if (g->debug == 0) printf("NO se agrego edge\n");
        if (g->debug == 0) graph_print(g);
        return;
    }
    Pair *data = calloc(1, sizeof(struct strPair));
    data->v = v;
    data->weight = weight;
    list_add(list_get(g->adjacencyList,idx_u), data);
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
        if (f_u != 0) f_u = g->cF(tV->n, u);
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
    Pair * tP = NULL;
    while (list_hasNext(it)) {
        it = (Iterator) list_next(it);
        p++;
        // Compara el elemento en el iterador con v
        tP = (Pair*)list_data(it);
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
        printf("\nNodo: "); g->pF(V0->n); printf("\n");
        printf("-Color: %c\n-Distancia: %f\n-Ti: %d\n-Tf: %d\n-Padre: ",V0->color,V0->dist,V0->Tdiscover,V0->Tdiscover);
        if(V0->parent) g->pF(V0->parent->n);
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



void BFS(Graph g, Type start) {/*

    //Queues de valores encontrados y procesados si aún tiene A por descubrir no se ha procesado
    Queue encontrados = queue_create(NULL);
    Queue procesados = queue_create(NULL);

    //inicia recorrido del grafo;
    Iterator dE = list_begin(g->adjacencyList);
    typedef struct strvertex * vertex;
    char blanco[] = "blanco";
    char negro[] = "negro";
    char gris[] = "gris";

    typedef struct{
        Node n;
        char color[6];
        int dist;
        vertex parent;
    }strvertex;

    vertex Vstart = calloc(1, sizeof(struct strvertex * ));
    Vstart->color = gris;
    Vstart->dist = 0;
    Vstart->parent = NULL;
    Vstart->node = list_get(list_data(dE),0);
    queue_offer(procesados, list_get(list_data(dE),0));
    //si lo activo reconoce el Vnex en las lineas 238 a 250
   /* vertex Vnext = calloc(1, sizeof(struct strvertex *));
    Vnext->color = blanco;
    Vnext->dist = 888;
    Vnext->parent = NULL;
    Vnext->node = list_get(list_data(dE), 0);*/
/*
    while (list_hasNext(dE)) {
        dE = (Iterator) list_next(dE);
        if (g->cF(list_get(list_data(dE), 0), start) = 0) {
            vertex Vnext = calloc(1, sizeof(struct strvertex *));
            Vnext->color = blanco;
            Vnext->dist = 888;
            Vnext->parent = NULL;
            Vnext->node = list_get(list_data(dE), 0);
            queue_offer(encontrados, list_get(list_data(dE), 0));
        }
    }
    while(!queue_isEmpty(encontrados)) {
        //Extraer el primer vértice de la cola: u
        Type Temp = queue_poll(encontrados);
        //Para cada vértice v en la lista de adyacencia de u:
        // Si v es blanco (no había sido descubierto)
        if(Vnext->color == blanco){
            // Cambiar el color de v a gris
            Vnext->color  = gris;
            // Asignar la distancia de v a (la distancia de u) + 1
            Vnext->dist = Vstart->dist +1;
            //Asignar u como el padre de v
            //falta poner el padre
            Vnext->parent = Vstart;
            //Encolar a v en q
            queue_offer(procesados,list_get(list_data(dE), 0));
            //Marcar a u como negro (ya procesado)
            Vnext->color=negro;
        }
    }*/
    /*Recorrido
      Mientras la cola no esté vacía…
*/

   /* //vertice inicial
    vertex Vs = calloc(1, sizeof(strvertex));
    strcpy(Vs->color, gris);
    Vs->dist = 0;
    vertex = (NULL);
    //no se si funciona así
    Node = NULL;*/

/*queue_offer(procesados, list_get(list_data(dS),0));

    while (list_hasNext(dE)) {
        dE = (Iterator) list_next(dE);
        if(g->cF(list_get(list_data(dE),0),start) = 0) {
            //resto de vertices
            vertex Vi = calloc(1, sizeof(strvertex));
            strcpy(vi->color, blanco);
            Vi.dist = 888;
            vertex = NULL;
            Node = n;
            //falta meter las estructuras en los nodos.
            queue_offer(blancos, list_get(list_data(dE), 0));
        }
    }
    while(queue_isEmpty(blancos) != 1) {
        //no estoy seguro que valor es
        int *p;
        p = queue_poll(blancos);
        while (list_hasNext(dE)) {
            if(vS->color == blanco){
                vS->color  == gris;
                //vi->dist = vs->dis + 1
                //parent = vs
                strcpy(vS->color, gris);
            }
            queue_offer(encontrados, list_get(list_data(dE), 0));
        }
    }*/
}
/* Setup de todos los vértices menos s
      Para todos los vertices en el grafo
          Su color es blanco
          Su distancia es infinita (algún valor muy grande que represente esto)
          Su padre es nulo (porque no ha sido descubierto)
	Setup del vértice s
      Su color es gris
      Su distancia es 0 (no hay que recorrer ninguna arista)
      Su padre es nulo (no hay otro vértice a partir del cual lo hayamos descubierto)
      Inicializamos la cola q  y le agregamos s*/


/*void funRecur(Graph g, vertex u){
    while (list_hasNext(dE)) {
        u.Tdiscover += 1;
        u.color = "gris";
    }*/
/* Función recursiva de visita (recibe al grafo y a un vértice u)
  El tiempo crece en 1
  Se asigna el tiempo como tiempo de descubrimiento de u
  Se asigna gris como el color de u (¡acaba de ser descubierto!)
  Por cada vértice v en la lista de adyacencia de u…*/


void DFS_recursive(Graph g, List u) {
    Vertex * uVtx = list_get(u,0);
    uVtx->Tdiscover = ++g->DFS_time;
    uVtx->color = 'G';
    if(list_size(u) > 1) {
        Iterator it = list_begin(u);
        it = (Iterator) list_next(it);
        Pair * tP = NULL;
        while (list_hasNext(it)) {
            it = (Iterator) list_next(it);
            tP = (Pair*)list_data(it);
            Type son = tP->v;

            Iterator dE = list_begin(g->adjacencyList);
            while (list_hasNext(dE)) {
                dE = (Iterator) list_next(dE);
                Vertex * tV = list_get(list_data(dE),0);
                if(g->cF(tV->n,son) == 0 && tV->color == 'B')
                    DFS_recursive(g,list_data(dE));
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
        List  Node = list_data(it);
        Vertex *u = list_get(list_data(it), 0);
        if(u->color == 'B') DFS_recursive(g, Node);
    }
    graph_print(g);
}



void dijkstra(Graph g, Type start) {

    //Crear un set que guarde todos los vertices calculados
    Queue gris = queue_create(NULL);
    Queue  negro = queue_create(NULL);
    Queue  blanca = queue_create(NULL);

    Iterator dE = list_begin(g->adjacencyList);
    while (list_hasNext(dE)) {
        dE = (Iterator) list_next(dE);
        if(g->cF(list_get(list_data(dE),0),start) != 0) {
            queue_offer(blanca, list_get(list_data(dE), 0));
            //falta agregar el la distancia infinita mediante las tablas hash y crear 3 mapas


        }
    }
    /*
        Inicialización (setup)
        Para cada vértice v…
            La distancia de v es “infinita”
            El padre de v es nulo
        La distancia del vértice de inicio s es 0 (y su padre es nulo)
    El conjunto S empieza vacío
    Insertar s en la cola de prioridad Q
    Mientras Q no esté vacía…
    Tomar el nodo mínimo de Q: u
    Agregar u al conjunto S
     Por cada vértice v en la lista de adyacencia de u y no contenido en S…
                                                i. 	Si la distancia de v es mayor que la suma de {la distancia de u} y {el peso del arista (u, v)}
    Asignar tal suma como la distancia de v
    Asignar a u como el padre de v (pues a través de u se puede llegar a v por una ruta más barata)


     Explicación de wiki
      Inicializar todas las distancias en D con un valor infinito relativo, ya que son desconocidas al principio,
     exceptuando la de x, que se debe colocar en 0, debido a que la distancia de x a x sería 0.

     Sea a = x (Se toma a como nodo actual.)

     Se recorren todos los nodos adyacentes de a, excepto los nodos marcados. Se les llamará nodos no marcados vi.

     Para el nodo actual, se calcula la distancia tentativa desde dicho nodo hasta sus vecinos con la siguiente
     fórmula: dt(vi) = Da + d(a,vi). Es decir, la distancia tentativa del nodo ‘vi’ es la distancia que actualmente
     tiene el nodo en el vector D más la distancia desde dicho nodo ‘a’ (el actual) hasta el nodo vi. Si la distancia
     tentativa es menor que la distancia almacenada en el vector, entonces se actualiza el vector con esta distancia
     tentativa. Es decir, si dt(vi) < Dvi → Dvi = dt(vi)

     Se marca como completo el nodo a.

     Se toma como próximo nodo actual el de menor valor en D (puede hacerse almacenando los valores en una cola de
     prioridad) y se regresa al paso 3, mientras existan nodos no marcados.
     */



}