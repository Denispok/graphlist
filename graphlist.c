#include <stdlib.h>
#include <stdio.h>
#include "graphlist.h"

void addNeighbour(struct Vertex *vertex, struct Vertex *neighbour);

void removeNeighbour(struct Vertex *vertex, struct Vertex *neighbour);

struct Graph *createGraph(int initialSize) {
    struct Graph *graph = (struct Graph *) malloc(sizeof(struct Graph));
    graph->size = 0;
    graph->maxSize = initialSize;
    graph->sizeMultiplier = 2;
    graph->vertexes = (struct Vertex *) malloc(sizeof(struct Vertex) * initialSize);
    return graph;
}

struct Graph *createGraphFromFile(char *fileName) {
    struct Graph *graph = createGraph(20);

    FILE *file = fopen(fileName, "rt");

    enum State {
        VERTEX, NODES
    } state = VERTEX;
    int wordSize = 0;
    int maxWordSize = 20;
    int currentVertexId;
    char *word = (char *) malloc(sizeof(char) * maxWordSize);

    int nextc = fgetc(file);
    while (nextc != EOF) {
        if (state == VERTEX) {
            if (nextc == ':') {
                if (wordSize != maxWordSize) word[wordSize] = '.';
                currentVertexId = atoi(word);
                struct Vertex *vertex = findVertexById(graph, currentVertexId);
                if (vertex == NULL) addVertex(graph, currentVertexId);
                wordSize = 0;
                state = NODES;
            } else {
                if (maxWordSize == wordSize) {
                    maxWordSize *= 2;
                    word = realloc(word, sizeof(char) * maxWordSize);
                }
                word[wordSize] = nextc;
                wordSize++;
            }
        } else {
            if (nextc == ' ');
            else if (nextc == ',') {
                if (wordSize != maxWordSize) word[wordSize] = '.';
                int vertexId = atoi(word);
                struct Vertex *vertex = findVertexById(graph, vertexId);
                if (vertex == NULL) addVertex(graph, vertexId);
                addEdge(graph, currentVertexId, vertexId);
                wordSize = 0;
            } else if (nextc == '\n') {
                if (wordSize != 0) {
                    if (wordSize != maxWordSize) word[wordSize] = '.';
                    int vertexId = atoi(word);
                    struct Vertex *vertex = findVertexById(graph, vertexId);
                    if (vertex == NULL) addVertex(graph, vertexId);
                    addEdge(graph, currentVertexId, vertexId);
                    wordSize = 0;
                }
                state = VERTEX;
            } else {
                if (maxWordSize == wordSize) {
                    maxWordSize *= 2;
                    word = realloc(word, sizeof(char) * maxWordSize);
                }
                word[wordSize] = nextc;
                wordSize++;
            }
        }
        nextc = fgetc(file);
    }
    if (wordSize != 0) {
        if (wordSize != maxWordSize) word[wordSize] = '.';
        int vertexId = atoi(word);
        struct Vertex *vertex = findVertexById(graph, vertexId);
        if (vertex == NULL) addVertex(graph, vertexId);
        addEdge(graph, currentVertexId, vertexId);
    }

    free(word);
    fclose(file);

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
        free(currentNode);
        return;
    }

    checkNextNode:
    if (currentNode->next == NULL) return;

    if (currentNode->next->vertex->id == neighbour->id) {
        struct Node *nextNode = currentNode->next;
        currentNode->next = currentNode->next->next;
        free(nextNode);
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

void printGraph(struct Graph *graph, char *fileName) {
    FILE *file = fileName == NULL ? stdout : fopen(fileName, "wt");

    for (int i = 0; i < graph->size; ++i) {
        fprintf(file, "%d: ", graph->vertexes[i].id);
        struct Node *currentNode = graph->vertexes[i].firstNeighbour;
        if (currentNode != NULL) {
            printNode:
            fprintf(file, "%d", currentNode->vertex->id);
            if (currentNode->next != NULL) {
                fprintf(file, ",");
                currentNode = currentNode->next;
                goto printNode;
            }
        }
        fprintf(file, "\n");
    }

    if (fileName != NULL) fclose(file);
}

void cleanGraph(struct Graph *graph) {
    for (int i = 0; i < graph->size; ++i) {
        struct Node *currentNode = graph->vertexes[i].firstNeighbour;
        struct Node *nextNode;
        while (currentNode != NULL) {
            nextNode = currentNode->next;
            free(currentNode);
            currentNode = nextNode;
        }
    }
    free(graph->vertexes);
    free(graph);
}
