#include "graph.h"
#include "bits/memoryfwd.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdio.h>

using namespace std;

#define V 0
#define DEBUG 0
#define LOGICAL_CHECK 0

void clean_graph(graph *&g) {
  delete[] g->ins;
  delete[] g->outs;
  delete[] g->in_degree_list;
  delete[] g->out_degree_list;
  delete[] g->matching;
}

int count_edges(FILE *&file, int &vertices) {
  int temp = 0;
  int src, dst;
  while (fscanf(file, "%d %d", &src, &dst) != EOF) {
    if ((src < vertices / 2 && dst >= vertices / 2) ||
        (src >= vertices / 2 && dst < vertices / 2)) {
      temp++;
    }
  }
  return temp;
}

void file_parser(FILE *&file, int *&srcs, int *&dsts, int &vertices) {
  int src, dst;
  int edges = 0;
  while (fscanf(file, "%d %d", &src, &dst) != EOF) {
    if ((src < vertices / 2 && dst >= vertices / 2) ||
        (src >= vertices / 2 && dst < vertices / 2)) {
#if DEBUG
      printf("adding %d->%d\n", src, dst);
#endif
      srcs[edges] = src;
      dsts[edges] = dst;
      edges++;
    }
  }
#if V
  printf("wrote %d edges into src and dst\n", edges);
#endif
}

void make_adjacency_list(graph *&g, int *&srcs, int *&dsts) {
#if V
  printf("starting to make adjacency list\n");
#endif
  int vertices = g->vertices;
  int edges = g->edges;
  int *ins = new int[g->edges];
  int *outs = new int[g->edges];
  int *out_deg_list = new int[g->vertices + 1];
  int *in_deg_list = new int[g->vertices + 1];

#if V
  printf("space allocated for adjacency list\n");
#endif

  for (unsigned i = 0; i < g->edges; ++i)
    outs[i] = 0;
  for (unsigned i = 0; i < g->edges; ++i)
    ins[i] = 0;
  for (unsigned int i = 0; i < g->vertices + 1; ++i)
    out_deg_list[i] = 0;
  for (unsigned int i = 0; i < g->vertices + 1; ++i)
    in_deg_list[i] = 0;

  int *temp_counts = new int[g->vertices];
  for (unsigned int i = 0; i < g->vertices; ++i)
    temp_counts[i] = 0;
  for (unsigned i = 0; i < g->edges; ++i)
    ++temp_counts[srcs[i]];
  for (unsigned int i = 0; i < g->vertices; ++i)
    out_deg_list[i + 1] = out_deg_list[i] + temp_counts[i];
  copy(out_deg_list, out_deg_list + g->vertices, temp_counts);
  for (unsigned i = 0; i < g->edges; ++i)
    outs[temp_counts[srcs[i]]++] = dsts[i];

  for (unsigned int i = 0; i < g->vertices; ++i)
    temp_counts[i] = 0;
  for (unsigned i = 0; i < g->edges; ++i)
    ++temp_counts[dsts[i]];
  for (unsigned int i = 0; i < g->vertices; ++i)
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

graph *make_graph(FILE *&file, int &verts) {
  graph *g = new graph;
  g->vertices = verts;
  int bi_edges;
  bi_edges = count_edges(file, verts);
  fseek(file, 0, SEEK_SET);
  g->edges = bi_edges;
#if LOGICAL_CHECK
  std::cout << "Bipartite graph has " << bi_edges << " edges" << endl;
#endif
  int *srcs = new int[bi_edges];
  int *dsts = new int[bi_edges];
#if DEBUG
  printf("allocated src and dst\n");
#endif
  file_parser(file, srcs, dsts, verts);
#if DEBUG
  printf("src and dst populated properly\n");
#endif
  make_adjacency_list(g, srcs, dsts);
#if DEBUG
  printf("adjacency list finished\n");
#endif
  g->matching = new int[verts];
  delete[] srcs;
  delete[] dsts;
  fseek(file, 0, SEEK_SET);
  return g;
}
