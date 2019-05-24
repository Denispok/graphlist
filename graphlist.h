
#ifndef GRAPHLIST_H
#define GRAPHLIST_H

struct Vertex {
    int id;
    struct Node *firstNeighbour;
};

struct Node {
    struct Vertex *vertex;
    struct Node *next;
};

struct Graph {
    struct Vertex *vertexes;
    int size;
    int maxSize;
    int sizeMultiplier;
};

struct Graph createGraph(int initialSize);

void addVertex(struct Graph *graph, int id);

void removeVertex(struct Graph *graph, int id);

struct Vertex *findVertexById(struct Graph *graph, int id);

void addNeighbour(struct Vertex *vertex, struct Vertex *neighbour);

void addEdge(struct Graph *graph, int vertexId1, int vertexId2);

void removeEdge(struct Graph *graph, int vertexId1, int vertexId2);

void printGraph(struct Graph *graph);

void cleanGraph(struct Graph *graph);

#endif //GRAPHLIST_H
