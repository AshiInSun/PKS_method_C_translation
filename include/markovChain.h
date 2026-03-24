//
// Created by Shaimani on 24/03/2026.
//

#ifndef PKS_METHOD_C_TRANSLATION_MARKOVCHAIN_H
#define PKS_METHOD_C_TRANSLATION_MARKOVCHAIN_H

#include "graph.h"

typedef struct MarkovChain{
    Graph graph;
    int N_swap;
    int gamma;
    bool force_k;

    //assortativity
    int assortativity;
    int D;

    //triangles

    //accept rate
    int accept_rate;
    int refusal_rate;

}MarkovChain;

void find_swap(MarkovChain* mc, int k);

void perform_swap(MarkovChain* mc, Edge* edges_to_swap, Edge* permutation, int* edge_to_swap_idx);

#endif //PKS_METHOD_C_TRANSLATION_MARKOVCHAIN_H
