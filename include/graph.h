//
// Created by shaim on 23/03/2026.
//

//
// graph.h
// Created by shaim on 23/03/2026.
//

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include <uthash.h>

// =====================
// Structures
// =====================

typedef struct {
    int u;
    int v;
} Edge;

typedef struct Edge_undirected {
    Edge e;
    int v_position_in_u;
    UT_hash_handle hh; // pour la hash table avec uthash
} Edge_undirected;

typedef struct Edge_directed {
    Edge e;
    int v_idx;
    int u_idx;
    int v_out_idx;
    int u_in_idx;
} Edge_directed;

typedef struct Neighbor {
    int* nodes;
    int count;
    int capacity;
} Neighbor;

typedef struct Graph {
    int N;                  // nombre de nœuds
    int M;                  // nombre d'arêtes
    bool is_directed;       // graphe dirigé
    int neighbor_capacity;
    int neighbor_count;
    Neighbor* neighbors;    // tableau de Neighbor
    Edge_directed* edges_directed;
    Edge_undirected* edges_undirected;
    Edge* unique_edges;
    int unique_count;
    int unique_capacity;
} Graph;

// =====================
// Initialisation
// =====================

void init_neighbor(Neighbor* neigh);
void init_neighbors(Graph* g, const int size);

// =====================
// Ajout d'arêtes
// =====================

void add_edge_to_neighbors(Graph* g, int u, int v);
void add_edge(Graph* g, int u, int v);
bool isEdgeEqual(Edge e, Edge f);

// =====================
// Lecture de fichiers
// =====================

int get_max_node(const char* filename);
void read_graph_ssv(Graph* g, const char* filename);
void print_edges_undirected(Graph* g);
#endif // GRAPH_H