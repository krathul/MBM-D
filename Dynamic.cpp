#include "Dynamic.h"
#include "Deletion.h"
#include "Insertion.h"
#include <cstdio>

void parse_update_file(FILE *file) {}

void graph_update(graph *&Bi_G, char *file) {
  FILE *update_file;
  update_file = fopen(file, "r");
  parse_update_file(update_file); // assuming we have parsed the update file
  int *potentail_sucessor;
  potentail_sucessor = new int[Bi_G->vertices];
  for (unsigned int i = 0; i < Bi_G->vertices; i++) {
    potentail_sucessor[i] = -1;
  }
  make_potential_paths();
  update_inserting_edges();
}
