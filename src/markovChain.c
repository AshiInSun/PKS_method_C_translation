//
// Created by Shaimani on 24/03/2026.
//

#include "markovChain.h"

void find_swap(MarkovChain* mc, int k){
    /**
    Randomly pick k edges to swap, and randomly pick a permutation
    When self.force_k == True, permutation is a cyclic permutation,
    else it is a random permutation, with possible identity for some edges.


            Parameters
            ----------
    k: int
    number of edges to swap

    Return
    ------
            edge_to_swap: list(tuples)
    list of the edges to swap
    permutation: list(tuples)
    list of the edges with which we should swap the\
                edges in edge_to_swap
    _edge_to_swap: list(int)
    indexes in unique_edges of the edges in edge_to_swap

    **/

    Edge* edge_to_swap = malloc(k * sizeof(Edge));
    for(int i = 0; i<k; i++){
        bool flag = true;
        Edge e;
        while(flag){
            flag = false;
            int idx = rand() % mc->graph.unique_count;
            e = mc->graph.unique_edges[idx];
            for(int j=0; j<i; j++){
                if(isEdgeEqual(e, edge_to_swap[j])){
                    flag = true;
                    break;
                }
            }
        }
        edge_to_swap[i] = e;
    }
}