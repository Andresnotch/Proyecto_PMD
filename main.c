#include <stdio.h>
#include "Graph.h"
#include "bool.h"

void printStr(Type val);

int AlphOrder(Type s1, Type s2);

int main() {
    Graph g = graph_create(printStr, NULL, AlphOrder);
    //graph_mode(g, 0);
    char *s1 = "Ganso";
    char *s2 = "Pato";
    char *s3 = "Conejo";
    char *s4 = "Kanguro";
    char *s5 = "Koala";
    char *s6 = "Panda";
    graph_addVertex(g, s1);
    graph_addVertex(g, s2);
    graph_addVertex(g, s3);
    graph_addVertex(g, s4);
    graph_addVertex(g, s5);
    graph_addVertex(g, s6);

    graph_addEdge(g, s5, s1, 7.9);
    graph_addEdge(g, s1, s2, 2.9);
    graph_addEdge(g, s2, s6, 2.1);
    graph_addEdge(g, s5, s4, 6.3);
    graph_addEdge(g, s6, s5, 4.8);
    graph_addEdge(g, s4, s3, 9.9);
    graph_addEdge(g, s5, s4, 6.7);
    graph_addEdge(g, s3, s6, 2.9);


    //BFS(g,s5);
    DFS(g);

    graph_destroy(g);
    return 0;
}

void printStr(Type val) {
    char *str = (char *) val;
    printf("%s", str);
}

int AlphOrder(Type s1, Type s2) {
    int cmp = 0, i = 0;
    char *str1 = (char *) s1, *str2 = (char *) s2;
    while (str1[i] != '\0' && str2[i] != '\0') {
        cmp = str1[i] - str2[i];
        if (cmp != 0) return cmp;
        i++;
    }
    if (str1[i] == '\0' && str2[i] != '\0') return str2[i];
    if (str1[i] != '\0' && str2[i] == '\0') return str1[i];
    return 0;
}

int hashstr(Type str, int m) {
    char *input = (char*) str;
    int total = 0, i = 0;
    while (input[i] != '\0')
        total += 17 * total + input[i++];
    total %= m;
    return total;
}