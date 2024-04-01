#include "Insertion.h"

void search_path(unsigned int vert, int *descendant, graph *&Bi_G) {
#if DEBUG
  printf("Searching path for %d\n", vert);
#endif
}

void make_potential_paths(graph *&Bi_G) {
  int *descendant;
  unsigned int vertices = Bi_G->vertices;
  descendant = new int[vertices];
  for (unsigned int i = 0; i < vertices; i++) {
    descendant[i] = -1;
  }
  for (int i = 0; i < vertices / 2; i++) {
    search_path(i, descendant);
  }
}

void update_inserting_edges(graph *&Bi_G) {}
