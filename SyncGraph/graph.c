//
// Created by yarin on 20/02/2024.
//

#include "graph.h"

node * createNode(vertex v) {
    node *newNode = malloc(sizeof(node));
    newNode->v = v;
    newNode->next = NULL;
    return newNode;
}

void addEdge(Graph *graph, vertex source, vertex destination) {
    node *node = createNode(destination);
    node->next = graph->adjacencyLists[source];
    graph->adjacencyLists[source] = node;

    node = createNode(source);
    node->next = graph->adjacencyLists[destination];
    graph->adjacencyLists[destination] = node;
}

Graph * createGraph(int vertices) {
    Graph *graph = malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->adjacencyLists = malloc(vertices * sizeof(node *));
    graph->num_visits = malloc(vertices * sizeof(vertex*));
    for(int i = 0 ; i < vertices ; i ++){
        graph->num_visits[i] = malloc(vertices * sizeof(vertex));
    }
    graph->num_visits_mutexes = malloc(vertices * sizeof(mutex));

    for (int i = 0; i < vertices; i++) {
        for( int j = 0; j < vertices ; j++) {
            graph->adjacencyLists[i] = NULL;
            graph->num_visits[i][j] = 0;
            pthread_mutex_init(&graph->num_visits_mutexes[i], NULL);
        }
    }

    return graph;
}


