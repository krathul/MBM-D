#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <string.h>

#include "Bi_partite_matching.h"
#include "Dynamic.h"
#include "graph.h"

using namespace std;

#define V 0
#define DEBUG 0
#define LOGIC_CHECK 0

int main(int argc, char* argv[])
{
    int thread_count = 4;

    omp_set_dynamic(0);
    omp_set_num_threads(thread_count);
    FILE* file;
    file = fopen(argv[1], "r");
    int u_verts = stoi(argv[2]);
    int v_verts = stoi(argv[3]);
    int verts = u_verts + v_verts;
    graph* Bi_G;
    Bi_G = make_graph(file, u_verts, v_verts);

    fclose(file);

    auto t1 = std::chrono::high_resolution_clock::now();
    match(Bi_G);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto diff1 = std::chrono::duration<double, std::milli>(t2 - t1).count();
    std::cout << "static : " << diff1 << "ms\n";

#if DEBUG
    printf("ran matching sucessfully\n");
#endif
    // printf("Maximum cardinality = %d\n", Bi_G->cardinality);
#if LOGIC_CHECK
    for (unsigned int i = 0; i < Bi_G->vertices; i++) {
        printf("match of %d-->%d\n", i, Bi_G->matching[i]);
    }
#endif
#ifndef STATIC_MODE
    // printf("\nGoing to start with dynamic update\n");
    // std::cout<<Bi_G->cardinality<<"\n";
    char* update_file = argv[4];
    graph_update(Bi_G, update_file);
// std::cout<<Bi_G->cardinality<<"\n";
#endif
    // printf("Maximum cardinality = %d\n", Bi_G->cardinality);
    clean_graph(Bi_G);
    delete Bi_G;
}
