#include "graph.h"
#include "bits/memoryfwd.h"
#include <fstream>
#include <iostream>
#include <stdio.h>

using namespace std;

void clean_graph(graph *g) {
  delete[] g->ins;
  delete[] g->outs;
  delete[] g->in_degree_list;
  delete[] g->out_degree_list;
}

int count_edges(FILE *file, int vertices) {
  int temp = 0;
  int src, dst;
  while (fscanf(file, "%d %d", &src, &dst) != EOF) {
    if ((src < vertices / 2 && dst >= vertices / 2) ||
        (src >= vertices / 2 && dst < vertices / 2)) {
      temp++;
    }
  }
  std::cout << "Bipartite graph has " << temp << " edges" << endl;
  return temp;
}

void file_parser(FILE *file, int *srcs, int *dsts, int vertices) {
  int src, dst;
  int edges = 0;
  while (fscanf(file, "%d %d", &src, &dst) != EOF) {
    if ((src < vertices / 2 && dst >= vertices / 2) ||
        (src >= vertices / 2 && dst < vertices / 2)) {
      srcs[edges++] = src;
      dsts[edges++] = dst;
    }
  }
}

void make_adjacency_list(graph *g, int *srcs, int *dsts) {
  int *ins = new int[g->edges];
  int *outs = new int[g->edges];
  int *out_deg_list = new int[g->vertices + 1];
  int *in_deg_list = new int[g->vertices + 1];

  for (unsigned i = 0; i < g->edges; ++i)
    outs[i] = 0;
  for (unsigned i = 0; i < g->edges; ++i)
    ins[i] = 0;
  for (int i = 0; i < g->vertices + 1; ++i)
    out_deg_list[i] = 0;
  for (int i = 0; i < g->vertices + 1; ++i)
    in_deg_list[i] = 0;

  int *temp_counts = new int[g->vertices];
  for (int i = 0; i < g->vertices; ++i)
    temp_counts[i] = 0;
  for (unsigned i = 0; i < g->edges; ++i)
    ++temp_counts[srcs[i]];
  for (int i = 0; i < g->vertices; ++i)
    out_deg_list[i + 1] = out_deg_list[i] + temp_counts[i];
  copy(out_deg_list, out_deg_list + g->vertices, temp_counts);
  for (unsigned i = 0; i < g->edges; ++i)
    outs[temp_counts[srcs[i]]++] = dsts[i];

  for (int i = 0; i < g->vertices; ++i)
    temp_counts[i] = 0;
  for (unsigned i = 0; i < g->edges; ++i)
    ++temp_counts[dsts[i]];
  for (int i = 0; i < g->vertices; ++i)
    in_deg_list[i + 1] = in_deg_list[i] + temp_counts[i];
  copy(in_deg_list, in_deg_list + g->vertices, temp_counts);
  for (unsigned i = 0; i < g->edges; ++i)
    ins[temp_counts[dsts[i]]++] = srcs[i];

  delete[] temp_counts;

  g->ins = ins;
  g->outs = outs;
  g->in_degree_list = in_deg_list;
  g->out_degree_list = out_deg_list;
}

graph *make_graph(FILE *file, int verts) {
  graph *g;
  g->vertices = verts;
  int bi_edges;
  bi_edges = count_edges(file, verts);
  fseek(file, 0, SEEK_SET);
  g->edges = bi_edges;
  int *srcs = new int[g->edges];
  int *dsts = new int[g->edges];
  file_parser(file, srcs, dsts, verts);
  make_adjacency_list(g, srcs, dsts);
  delete[] srcs;
  delete[] dsts;
  fseek(file, 0, SEEK_SET);
  return g;
}
