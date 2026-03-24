//
// Created by shaim on 23/03/2026.
//
#include <stdio.h>
#include "graph.h"

int main() {
    const char* filename = "../data/toyexample.out";

    int max_node = get_max_node(filename);
    if (max_node < 0) return 1;

    Graph g;
    g.N = max_node+1;
    g.M = 0;
    g.is_directed = false;
    g.neighbors = NULL;
    g.edges_undirected = NULL;
    g.unique_edges = NULL;
    g.unique_count = 0;
    g.unique_capacity = 0;

    init_neighbors(&g, g.N);

    read_graph_ssv(&g, filename);

    // =====================
    // 🔹 Affichage neighbors
    // =====================
    printf("=== Neighbors ===\n");
    for (int i = 0; i < g.N; i++) {
        printf("Node %d: ", i);
        for (int j = 0; j < g.neighbors[i].count; j++)
            printf("%d ", g.neighbors[i].nodes[j]);
        printf("\n");
    }

    // =====================
    // 🔹 Affichage unique_edges
    // =====================
    printf("\n=== Unique edges ===\n");
    for (int i = 0; i < g.unique_count; i++) {
        printf("(%d, %d)\n", g.unique_edges[i].u, g.unique_edges[i].v);
    }

    // =====================
    // 🔹 Vérification cohérence
    // =====================
    printf("\n=== Checking consistency ===\n");

    int errors = 0;

    for (int i = 0; i < g.unique_count; i++) {
        int u = g.unique_edges[i].u;
        int v = g.unique_edges[i].v;

        int found = 0;
        for (int j = 0; j < g.neighbors[u].count; j++) {
            if (g.neighbors[u].nodes[j] == v) {
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("Error: edge (%d,%d) missing in neighbors[%d]\n", u, v, u);
            errors++;
        }
    }

    if (errors == 0)
        printf("All checks passed ✅\n");
    else
        printf("Found %d errors ❌\n", errors);
    print_edges_undirected(&g);

    // =====================
    // 🔹 Stats
    // =====================
    printf("\n=== Stats ===\n");
    printf("N = %d\n", g.N);
    printf("M (unique edges) = %d\n", g.M);

    return 0;
}