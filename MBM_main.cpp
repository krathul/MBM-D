#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <string.h>

using namespace std;

#include "Bi_partite_matching.h"
#include "graph.h"

#define V 0
#define DEBUG 0
#define LOGIC_CHECK 0

int main(int argc, char *argv[]) {
  omp_set_dynamic(0);
  omp_set_num_threads(stoi(argv[3]));
#if V
  printf("give number of vertices as argument 2 and file in 1\n");
#endif
  FILE *file;
  file = fopen(argv[1], "r");
  int verts = stoi(argv[2]);
  graph *Bi_G;
  Bi_G = make_graph(file, verts);
  fclose(file);
#if DEBUG
  printf("graph is made successfully\n");
#endif
  double start = omp_get_wtime();
  match(Bi_G);
  double end = omp_get_wtime();
  printf("Ran in %f seconds\n", end - start);
#if DEBUG
  printf("ran matching sucessfully\n");
#endif
  printf("Maximum cardinality = %d\n", Bi_G->cardinality);
  clean_graph(Bi_G);
  delete Bi_G;
}
