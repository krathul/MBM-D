#include "Dynamic.h"
#include "Deletion.h"
#include "Insertion.h"
#include <cstdio>

void parse_update_file(FILE *file) {}

void graph_update(graph *&Bi_G, char *file) {
  FILE *update_file;
  update_file = fopen(file, "r");
  parse_update_file(update_file); // assuming we have parsed the update file
  int *descendant, *end_points;
  make_potential_paths(Bi_G, descendant, end_points);
  update_inserting_edges();
}
