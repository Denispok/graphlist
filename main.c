#include <stdio.h>
#include "graphlist.h"

int addEdgeTest();

int removeEdgeTest();

int addVertexTest();

int removeVertexTest();

int main() {
    struct Graph graph = createGraph(10);
    for (int i = 0; i <= 40; ++i) {
        addVertex(&graph, i);
    }
    for (int i = 0; i <= 40; ++i) {
        addEdge(&graph, i, 10);
        addEdge(&graph, i, 25);
    }
    removeEdge(&graph, 10, 40);
    addEdge(&graph, 40, 10);
    removeEdge(&graph, 25, 40);
    printGraph(&graph);

    // Tests
    printf("addEdgeTest: %s\n", !addEdgeTest() ? "success" : "error");
    printf("removeEdgeTest: %s\n", !removeEdgeTest() ? "success" : "error");
    printf("addVertexTest: %s\n", !addVertexTest() ? "success" : "error");
    printf("removeVertexTest: %s\n", !removeVertexTest() ? "success" : "error");

    return 0;
}

struct Graph createTestGraph(int from, int to) {
    struct Graph graph = createGraph(to - from + 1);
    graph.size = to - from + 1;
    for (int i = 0; i <= to - from; ++i) {
        struct Vertex vertex;
        vertex.id = from++;
        vertex.firstNeighbour = NULL;
        graph.vertexes[i] = vertex;
    }
    return graph;
}

int addEdgeTest() {
    struct Graph graph = createTestGraph(1, 10);
    struct Node *node;

    addEdge(&graph, 1, 2);
    struct Vertex *vertex1 = &graph.vertexes[0];
    struct Vertex *vertex2 = &graph.vertexes[1];
    node = vertex1->firstNeighbour;
    if (!(node != NULL &&
          node->vertex == vertex2 &&
          node->next == NULL))
        return -1;
    node = vertex2->firstNeighbour;
    if (!(node != NULL &&
          node->vertex == vertex1 &&
          node->next == NULL))
        return -1;

    addEdge(&graph, 1, 2);
    node = vertex1->firstNeighbour;
    if (!(node != NULL &&
          node->vertex == vertex2 &&
          node->next == NULL))
        return -1;
    node = vertex2->firstNeighbour;
    if (!(node != NULL &&
          node->vertex == vertex1 &&
          node->next == NULL))
        return -1;

    struct Vertex *vertex3 = &graph.vertexes[2];
    addEdge(&graph, 1, 3);
    node = vertex1->firstNeighbour;
    if (!(node != NULL &&
          ((node->vertex == vertex2 && node->next != NULL && node->next->vertex == vertex3) ||
           (node->vertex == vertex3 && node->next != NULL && node->next->vertex == vertex2)) &&
          node->next->next == NULL))
        return -1;
    node = vertex3->firstNeighbour;
    if (!(node != NULL &&
          node->vertex == vertex1 &&
          node->next == NULL))
        return -1;

    return 0;
}

int removeEdgeTest() {
    struct Graph graph = createTestGraph(1, 10);
    struct Vertex *vertex1 = &graph.vertexes[0];
    struct Node node1;
    node1.vertex = vertex1;
    node1.next = NULL;
    struct Vertex *vertex2 = &graph.vertexes[1];
    struct Node node2;
    node2.vertex = vertex2;
    node2.next = NULL;
    struct Vertex *vertex3 = &graph.vertexes[2];
    struct Node node3;
    node3.vertex = vertex3;
    node3.next = NULL;

    vertex1->firstNeighbour = &node2;
    vertex2->firstNeighbour = &node1;
    removeEdge(&graph, 1, 2);
    if (!(vertex1->firstNeighbour == NULL &&
          vertex2->firstNeighbour == NULL))
        return -1;

    vertex1->firstNeighbour = &node2;
    vertex1->firstNeighbour->next = &node3;
    vertex2->firstNeighbour = &node1;
    vertex3->firstNeighbour = &node1;
    removeEdge(&graph, 1, 2);
    if (!(vertex1->firstNeighbour == &node3 &&
          vertex1->firstNeighbour->next == NULL &&
          vertex2->firstNeighbour == NULL &&
          vertex3->firstNeighbour == &node1 &&
          vertex3->firstNeighbour->next == NULL))
        return -1;

    vertex1->firstNeighbour = &node2;
    vertex1->firstNeighbour->next = &node3;
    vertex2->firstNeighbour = &node1;
    vertex3->firstNeighbour = &node1;
    removeEdge(&graph, 1, 3);
    if (!(vertex1->firstNeighbour == &node2 &&
          vertex1->firstNeighbour->next == NULL &&
          vertex2->firstNeighbour == &node1 &&
          vertex2->firstNeighbour->next == NULL &&
          vertex3->firstNeighbour == NULL))
        return -1;

    return 0;
}

int addVertexTest() {
    struct Graph graph = createGraph(4);
    int size = 0;

    for (int i = 0; i < 16; ++i) {
        addVertex(&graph, i);
        if (graph.size != ++size) return -1;
        for (int j = 0; j <= i; ++j) {
            if (graph.vertexes[j].id == i) goto vertexIsFound;
        }
        return -1;
        vertexIsFound:;
    }

    return 0;
}

int removeVertexTest() {
    struct Graph graph = createTestGraph(1, 10);
    struct Node node1;
    struct Node node2;
    struct Vertex *vertex1 = &graph.vertexes[0];
    node1.vertex = vertex1;
    node1.next = NULL;
    vertex1->firstNeighbour = &node2;
    struct Vertex *vertex2 = &graph.vertexes[1];
    node2.vertex = vertex2;
    node2.next = NULL;
    vertex2->firstNeighbour = &node1;

    removeVertex(&graph, 1);
    if (graph.size != 9) return -1;
    for (int i = 0; i < 9; ++i) {
        if (graph.vertexes[i].id == 1) return -1;
        if (graph.vertexes[i].firstNeighbour == &node1) return -1;
    }

    return 0;
}
