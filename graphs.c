#include <stdlib.h>
#include "graphs.h"

Graph from_DIMACS(char *filename) {
    Graph G;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return NULL;

    char H;
    size_t a, b;
    char comments[100];

    do {
        H = fgetc(fp);

        switch (H)
        {
        case 'p':
            fscanf(fp, " %s %zu %zu\n",comments, &a, &b);
            G = new_graph(a);
            break;
        
        case 'e':
            fscanf(fp, " %zu %zu\n", &a, &b);
            add_edge(G, a-1, b-1);
            break;

        default:
            fgets(comments, 80, fp);
            printf("%s\n", comments);
            break;
        }
    } while (H!=EOF);
    fclose(fp);
    return G;
}

// constructor
Graph new_digraph(size_t n_nodes) {
    Graph G = (Graph)malloc(sizeof(struct _GRAPH));
    G->is_directed = 1;
    G->n_nodes = n_nodes;
    G->degrees = (size_t *)malloc(sizeof(size_t)*n_nodes); // outer degrees only
    // initialize degrees to 0
    for (size_t i=0; i<n_nodes; i++) {
        G->degrees[i] = 0;
    }

    G->nodes = (size_t **)malloc(sizeof(size_t *)*n_nodes);
}

Graph new_graph(size_t n_nodes) {
    Graph G = (Graph)malloc(sizeof(struct _GRAPH));
    G->is_directed = 0;
    G->n_nodes = n_nodes;
    G->degrees = (size_t *)malloc(sizeof(size_t)*n_nodes); // outer degrees only

    // initialize degrees to 0
    for (size_t i=0; i<n_nodes; i++) {
        G->degrees[i] = 0;
    }
    G->nodes = (size_t **)malloc(sizeof(size_t *)*n_nodes);
}

// basic reporting and manipulation
void add_edge(Graph G, size_t source, size_t dest)
{

    // avoid self loop
    if (source == dest) return;

    if (G->degrees[source]) {
        // copy old adj list
        size_t tmp[G->degrees[source]];
        for (size_t i=0; i<G->degrees[source]; i++) {
            tmp[i] = G->nodes[source][i];
        }
        G->degrees[source]++;
        G->nodes[source] = (size_t *)realloc(G->nodes[source], G->degrees[source]*sizeof(size_t));
    }
    else {
        G->degrees[source]++;
        G->nodes[source] = (size_t *)malloc(sizeof(size_t));
    }
    G->nodes[source][G->degrees[source]-1] = dest;

    // make reverse edge adding in case of unidrected graph
    if (!G->is_directed) {
        if (G->degrees[dest]) {
            // copy old adj list
            size_t tmp[G->degrees[dest]];
            for (size_t i=0; i<G->degrees[dest]; i++) {
                tmp[i] = G->nodes[dest][i];
            }
            G->degrees[dest]++;
            G->nodes[dest] = (size_t *)realloc(G->nodes[dest], G->degrees[dest]*sizeof(size_t));
        }
        else {
            G->degrees[dest]++;
            G->nodes[dest] = (size_t *)malloc(sizeof(size_t));
        }
        G->nodes[dest][G->degrees[dest]-1] = source;
    }
}

void del_node(Graph G, size_t node);
void del_edge(Graph G, size_t source, size_t destination);

size_t *successors(Graph G, size_t node);
size_t *predecessors(Graph G, size_t node);

// basic visualisation

void print_graph(Graph G) {
    printf("%zu nodes\n", G->n_nodes);
    for (size_t source=0; source<G->n_nodes; source++) {
        printf("%zu:", source);
        for (size_t dest_idx=0; dest_idx<G->degrees[source]; dest_idx++) {
            printf("%zu;", G->nodes[source][dest_idx]);
        }
        printf("\n");
    }
}

List shortest_path(Graph G, size_t node) {

    size_t distance[G->n_nodes];

    // initialisation
    for (size_t i=0; i<G->n_nodes; i++) distance[i] = (size_t)(-1);

    // printf("A\n");

    List to_explore = new_list();
    push(to_explore, node);
    distance[node] = 0;
    
    // printf("B\n");

    while (!is_empty(to_explore)) {

        size_t current_node = get(to_explore, 0);
        size_t s;
        size_t current_distance = distance[current_node];

        pop(to_explore, 0);

        // printf("plop\n");

        for (size_t i=0; i<G->degrees[current_node]; i++) {
            s = G->nodes[current_node][i];
            if (distance[s] == (size_t)(-1)) {
                distance[s] = current_distance + 1;
                push(to_explore, s);
            }
        }

    }

    // printf("C\n");

    destroy(to_explore);

    List result = new_list();
    for (size_t i=0; i<G->n_nodes; i++) {
        push(result, distance[i]);
    }
    return result;
}