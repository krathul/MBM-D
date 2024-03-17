#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

#include "Bi_partite_matching.h"
#include "graph.h"

#define DEBUG 0
#define LOGIC_CHECK 0

int main(int argc, char *argv[]) {
  printf("give number of vertices as argument 2 and file in 1\n");
  cout << "First line" << endl;
  FILE *file;
  file = fopen(argv[1], "r");
  int verts = stoi(argv[2]);
  graph *Bi_G;
  Bi_G = make_graph(file, verts);
  fclose(file);
#if DEBUG
  printf("graph is made successfully\n");
#endif
  match(Bi_G);
#if DEBUG
  printf("ran matching sucessfully\n");
#endif
  printf("Maximum cardinality = %d\n", Bi_G->cardinality);
  clean_graph(Bi_G);
  delete Bi_G;
}
