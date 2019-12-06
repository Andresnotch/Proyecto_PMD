#include <stdio.h>
#include "Graph.h"

void printStr(Type val);

int AlphOrder(Type s1, Type s2);

int main() {
    Graph g = graph_create(printStr, NULL, AlphOrder);
    graph_mode(g, 0);
    char *s1 = "Ganso";
    char *s2 = "Pato";
    graph_addVertex(g, s1);
    graph_addVertex(g, s2);

    graph_addEdge(g, s1, s2, 2.9);

    graph_deleteEdge(g, s1, s2);


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
    if ((str1[i] == '\0' && str2[i] != '\0')) return str2[i];
    if ((str1[i] != '\0' && str2[i] == '\0')) return str1[i];
    return 0;
}