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
    if (g->is_directed) {

        // on fera plus tard

    } else {

        // normaliser pour unique_edges : u < v
        int u_norm = u;
        int v_norm = v;

        if (u_norm > v_norm) {
            int tmp = u_norm;
            u_norm = v_norm;
            v_norm = tmp;
        }

        Edge key_unique = {u_norm, v_norm};
        Edge_undirected* e_check;

        // vérifier si l'arête unique existe déjà
        HASH_FIND(hh, g->edges_undirected,
                  &key_unique, sizeof(Edge), e_check);

        if (e_check != NULL) {
            // edge déjà présente
            return;
        }

        // -----------------------------
        // trouver position de v dans neighbors[u]
        // -----------------------------

        int pos_uv = -1;
        for (int i = 0; i < g->neighbors[u].count; i++) {
            if (g->neighbors[u].nodes[i] == v) {
                pos_uv = i;
                break;
            }
        }

        if (pos_uv == -1) {
            fprintf(stderr,
                    "Error: edge (%d, %d) not found in neighbors\n",
                    u, v);
            return;
        }

        // -----------------------------
        // trouver position de u dans neighbors[v]
        // -----------------------------

        int pos_vu = -1;
        for (int i = 0; i < g->neighbors[v].count; i++) {
            if (g->neighbors[v].nodes[i] == u) {
                pos_vu = i;
                break;
            }
        }

        if (pos_vu == -1) {
            fprintf(stderr,
                    "Error: edge (%d, %d) not found in neighbors\n",
                    v, u);
            return;
        }

        // -----------------------------
        // gérer unique_edges (tableau dynamique)
        // -----------------------------

        if (g->unique_capacity == 0) {
            g->unique_capacity = 4;

            g->unique_edges =
                    malloc(g->unique_capacity * sizeof(Edge));

            if (!g->unique_edges) {
                perror("malloc");
                exit(1);
            }

        } else if (g->unique_count == g->unique_capacity) {

            g->unique_capacity *= 2;

            Edge* tmp =
                    realloc(g->unique_edges,
                            g->unique_capacity * sizeof(Edge));

            if (!tmp) {
                perror("realloc");
                exit(1);
            }

            g->unique_edges = tmp;
        }

        // ajouter arête unique
        g->unique_edges[g->unique_count++] = key_unique;
        g->M++;

        // -----------------------------
        // ajouter (u,v) dans hash table
        // -----------------------------

        Edge_undirected* e_uv =
                malloc(sizeof(Edge_undirected));

        if (!e_uv) {
            perror("malloc");
            exit(1);
        }

        e_uv->e.u = u;
        e_uv->e.v = v;
        e_uv->v_position_in_u = pos_uv;

        HASH_ADD(hh,
                 g->edges_undirected,
                 e,
                 sizeof(Edge),
                 e_uv);

        // -----------------------------
        // ajouter (v,u) dans hash table
        // -----------------------------

        Edge_undirected* e_vu =
                malloc(sizeof(Edge_undirected));

        if (!e_vu) {
            perror("malloc");
            exit(1);
        }

        e_vu->e.u = v;
        e_vu->e.v = u;
        e_vu->v_position_in_u = pos_vu;

        HASH_ADD(hh,
                 g->edges_undirected,
                 e,
                 sizeof(Edge),
                 e_vu);
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