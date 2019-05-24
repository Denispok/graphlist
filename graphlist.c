#include <stdlib.h>
#include <stdio.h>
#include "graphlist.h"

void addNeighbour(struct Vertex *vertex, struct Vertex *neighbour);

void removeNeighbour(struct Vertex *vertex, struct Vertex *neighbour);

struct Graph createGraph(int initialSize) {
    struct Graph graph;
    graph.size = 0;
    graph.maxSize = initialSize;
    graph.sizeMultiplier = 2;
    graph.vertexes = (struct Vertex *) malloc(sizeof(struct Vertex) * initialSize);
    return graph;
}

void addVertex(struct Graph *graph, int id) {
    if (graph->size == graph->maxSize) {
        graph->maxSize = graph->maxSize * graph->sizeMultiplier;
        int newMemSize = sizeof(struct Vertex) * graph->maxSize;
        graph->vertexes = (struct Vertex *) realloc(graph->vertexes, newMemSize);
    }
    graph->vertexes[graph->size].id = id;
    graph->vertexes[graph->size].firstNeighbour = NULL;
    graph->size++;
}

void removeVertex(struct Graph *graph, int id) {
    struct Vertex *vertex = findVertexById(graph, id);

    struct Node *nextNode = vertex->firstNeighbour;
    while (nextNode != NULL) {
        removeEdge(graph, vertex->id, nextNode->vertex->id);
        nextNode = nextNode->next;
    }

    struct Vertex *lastVertex = &(graph->vertexes[graph->size - 1]);
    vertex->id = lastVertex->id;
    vertex->firstNeighbour = lastVertex->firstNeighbour;
    graph->size--;
}

struct Vertex *findVertexById(struct Graph *graph, int id) {
    for (int i = 0; i < graph->size; ++i) {
        if (graph->vertexes[i].id == id) return &(graph->vertexes[i]);
    }
    return NULL;
}

void addNeighbour(struct Vertex *vertex, struct Vertex *neighbour) {
    struct Node *lastNode = vertex->firstNeighbour;
    if (lastNode == NULL) {
        vertex->firstNeighbour = (struct Node *) malloc(sizeof(struct Node));
        vertex->firstNeighbour->vertex = neighbour;
        vertex->firstNeighbour->next = NULL;
    } else {
        checkNextNode:
        if (lastNode->vertex->id == neighbour->id) return;
        if (lastNode->next != NULL) {
            lastNode = lastNode->next;
            goto checkNextNode;
        }
        lastNode->next = (struct Node *) malloc(sizeof(struct Node));
        lastNode->next->vertex = neighbour;
        lastNode->next->next = NULL;
    }
}

void removeNeighbour(struct Vertex *vertex, struct Vertex *neighbour) {
    struct Node *currentNode = vertex->firstNeighbour;
    if (currentNode == NULL) return;

    if (currentNode->vertex->id == neighbour->id) {
        vertex->firstNeighbour = currentNode->next;
        return;
    }

    checkNextNode:
    if (currentNode->next == NULL) return;

    if (currentNode->next->vertex->id == neighbour->id) {
        currentNode->next = currentNode->next->next;
    } else {
        currentNode = currentNode->next;
        goto checkNextNode;
    }
}

void addEdge(struct Graph *graph, int vertexId1, int vertexId2) {
    struct Vertex *vertex1 = findVertexById(graph, vertexId1);
    struct Vertex *vertex2 = findVertexById(graph, vertexId2);
    addNeighbour(vertex1, vertex2);
    addNeighbour(vertex2, vertex1);
}

void removeEdge(struct Graph *graph, int vertexId1, int vertexId2) {
    struct Vertex *vertex1 = findVertexById(graph, vertexId1);
    struct Vertex *vertex2 = findVertexById(graph, vertexId2);
    removeNeighbour(vertex1, vertex2);
    removeNeighbour(vertex2, vertex1);
}

void printGraph(struct Graph *graph) {
    for (int i = 0; i < graph->size; ++i) {
        printf("%d: ", graph->vertexes[i].id);
        struct Node *currentNode = graph->vertexes[i].firstNeighbour;
        if (currentNode != NULL) {
            printNode:
            printf("%d", currentNode->vertex->id);
            if (currentNode->next != NULL) {
                printf(",");
                currentNode = currentNode->next;
                goto printNode;
            }
        }
        printf("\n");
    }
}
