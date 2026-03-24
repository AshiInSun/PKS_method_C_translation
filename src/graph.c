//
// Created by shaim on 23/03/2026.
//

#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "uthash.h"

void init_neighbor(Neighbor* neigh) {
    neigh->nodes = NULL;
    neigh->count = 0;
    neigh->capacity = 0;
}

void init_neighbors(Graph* g, const int size) {
    g->neighbors = malloc(size * sizeof(Neighbor));
    for (int i = 0; i < size; i++) {
        init_neighbor(&g->neighbors[i]);
    }
}

void add_edge_to_neighbors(Graph* g, int u, int v) {
    //We add v to the neighbors of u

    Neighbor* neigh = &g->neighbors[u];
    if(neigh->nodes == NULL)
    {
        neigh->capacity = 4;
        neigh->nodes = malloc(neigh->capacity * sizeof(int));
    }
    // Check if v is already a neighbor of u
    //TODO : binary search ?
    for (int i = 0; i < neigh->count; i++) {
        if(neigh->nodes[i] == v)
            return;
    }
    if(neigh->count == neigh->capacity)
    {
        neigh->capacity *= 2;
        int* tmp = realloc(neigh->nodes, neigh->capacity * sizeof(int));
        if(tmp == NULL) { perror("realloc"); exit(1); }
        neigh->nodes = tmp;
    }
    neigh->nodes[neigh->count] = v;
    neigh->count++;


    //We add u to the neighbors of v
    Neighbor* neighv = &g->neighbors[v];
    if(neighv->nodes == NULL)
    {
        neighv->capacity = 4;
        neighv->nodes = malloc(neighv->capacity * sizeof(int));
    }
    // Check if u is already a neighbor of v
    for (int i = 0; i < neigh->count; i++) {
        if(neighv->nodes[i] == u)
            return;
    }
    if(neighv->count == neigh->capacity)
    {
        neighv->capacity *= 2;
        int* tmp = realloc(neighv->nodes, neighv->capacity * sizeof(int));
        if(tmp == NULL) { perror("realloc"); exit(1); }
        neighv->nodes = tmp;
    }
    neighv->nodes[neighv->count] = u;
    neighv->count++;
}

void add_edge(Graph* g, int u, int v) {
    if(g->is_directed)
    {

    }else{
        if (u > v) {
            int tmp = u;
            u = v;
            v = tmp;
        }

        Edge key = {u, v};
        Edge_undirected* e;

        int pos = -1;
        for (int i = 0; i < g->neighbors[u].count; i++) {
            if (g->neighbors[u].nodes[i] == v) {
                pos = i;
                break;
            }
        }
        if (pos == -1) {
            fprintf(stderr, "Error: edge (%d, %d) not found in neighbors\n", u, v);
            return;
        }

        // vérifier si existe
        HASH_FIND(hh, g->edges_undirected, &key, sizeof(Edge), e);
        if (e != NULL) return;

        // créer nouvelle arête
        e = malloc(sizeof(Edge_undirected));
        if (!e) { perror("malloc"); exit(1); }
        e->e.u = u;
        e->e.v = v;
        e->v_position_in_u = pos;
        // redimensionner si nécessaire
        if (g->unique_capacity == 0) {
            g->unique_capacity = 4;
            g->unique_edges = malloc(g->unique_capacity * sizeof(Edge));
            if (!g->unique_edges) { perror("malloc"); exit(1); }
        }
        else if (g->unique_count == g->unique_capacity) {
            g->unique_capacity *= 2;
            Edge* tmp = realloc(g->unique_edges, g->unique_capacity * sizeof(Edge));
            if (!tmp) { perror("realloc"); exit(1); }
            g->unique_edges = tmp;
        }
        g->M++;
        Edge unique = key;
        g->unique_edges[g->unique_count++] = unique;
        // ajouter dans la hash table
        HASH_ADD(hh, g->edges_undirected, e, sizeof(Edge), e);
    }
}


//METHODS TO READ FILES

int get_max_node(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) { perror("fopen"); return -1; }

    int max_node = -1;
    int u, v;

    while (fscanf(f, "%d %d", &u, &v) == 2) {
        if (u > max_node) max_node = u;
        if (v > max_node) max_node = v;
    }

    fclose(f);
    return max_node;
}

void read_graph_ssv(Graph* g, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) { perror("fopen"); return; }

    int u, v;
    while (fscanf(f, "%d %d", &u, &v) == 2) {
        add_edge_to_neighbors(g, u, v);
    }
    fclose(f);

    f = fopen(filename, "r");
    if (!f) { perror("fopen"); return; }

    while (fscanf(f, "%d %d", &u, &v) == 2) {
        add_edge(g, u, v);
    }
    fclose(f);
}
void print_edges_undirected(Graph* g) {
    Edge_undirected *e, *tmp;

    printf("\n=== Edges (hash table) ===\n");

    HASH_ITER(hh, g->edges_undirected, e, tmp) {
        printf("(%d, %d) -> pos_in_u = %d\n",
               e->e.u, e->e.v, e->v_position_in_u);
    }
}