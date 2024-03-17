#include <string>

#ifndef GRAPH_H
#define GRAPH_H

typedef struct graph {
  unsigned vertices;
  unsigned edges;
  int *ins;
  int *outs;
  int *in_degree_list;
  int *out_degree_list;
  int *matching;
  unsigned int cardinality;
} graph;

graph *make_graph(FILE *&file, int &verts);

void clean_graph(graph *&g);

#endif
