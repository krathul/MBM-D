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

int main(int argc, char *argv[]) {
  int thread_count = 2;
  if (argc == 1) {
    printf("=>first arg graph_file \n=>second arg vertices \n=>third arg "
           "update_file "
           "\n=>optional fourth arg is thread\n");
    exit(1);
  }
  if (argc == 5) {
    thread_count = stoi(argv[4]);
  }
  printf("Running with %d threads\n", thread_count);
  omp_set_dynamic(0);
  omp_set_num_threads(thread_count);
  FILE *file;
  file = fopen(argv[1], "r");
  int verts = stoi(argv[2]);
  graph *Bi_G;
  Bi_G = make_graph(file, verts);
  printf("Graph has %d vertices\n", Bi_G->vertices);
  fclose(file);
  double start = omp_get_wtime();
  match(Bi_G);
  double end = omp_get_wtime();
  printf("Ran in %f seconds\n", end - start);
#if DEBUG
  printf("ran matching sucessfully\n");
#endif
  printf("Maximum cardinality = %d\n", Bi_G->cardinality);
#if LOGIC_CHECK
  for (unsigned int i = 0; i < Bi_G->vertices; i++) {
    printf("match of %d-->%d\n", i, Bi_G->matching[i]);
  }
#endif

  printf("\nGoing to start with dynamic update\n");
  graph_update(Bi_G, argv[3]);
  printf("Maximum cardinality = %d\n", Bi_G->cardinality);
  clean_graph(Bi_G);
  delete Bi_G;
}
