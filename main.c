#include <stdio.h>
#include "graphlist.h"

int main() {
    struct Graph graph = createGraph(10);
    for (int i = 0; i <= 40; ++i) {
        addVertex(&graph, i);
    }
    for (int i = 0; i <= 40; ++i) {
        addEdge(&graph, i, 10);
        addEdge(&graph, i, 25);
    }
    printGraph(&graph);
    return 0;
}
