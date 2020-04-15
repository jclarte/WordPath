#ifndef GRAPHS_H
#define GRAPHS_H

#include <stdlib.h>
#include <stdio.h>
#include "lists.h"

typedef struct _GRAPH {
    int is_directed;
    size_t n_nodes;
    size_t *degrees;
    size_t **nodes;
} *Graph;

// DIMACS parser
Graph from_DIMACS(char *filename);

// constructor
Graph new_graph(size_t n_nodes);
Graph new_digraph(size_t n_nodes);

// basic reporting and manipulation
void add_edge(Graph G, size_t source, size_t dest);

void del_node(Graph G, size_t node);
void del_edge(Graph G, size_t source, size_t destination);

size_t *neighbors(Graph G, size_t node);
size_t *successors(Graph G, size_t node);
size_t *predecessors(Graph G, size_t node);

// basic visualisation
void print_graph(Graph G);

// simple algorithms
List shortest_path(Graph G, size_t node);

#endif