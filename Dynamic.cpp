#include "Dynamic.h"
#include "Deletion.h"
#include "Insertion.h"
#include <Modified_Bi_partite_matching.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <omp.h>
#include <stdio.h>

#define DEBUG 0

#define valid_verts(V1, V2, N)                                                 \
  ((V1 < N / 2 && N / 2 <= V2) || (N / 2 <= V1 && V2 < N / 2))

void count_updates(FILE *file, unsigned int &D_count, unsigned int &I_count,
                   unsigned int &Verts) {
  char op;
  unsigned int V1, V2;
  while (fscanf(file, "%c%*[ \t]%u%*[ \t]%u", &op, &V1, &V2) != EOF) {
    if ('a' == op && (valid_verts(V1, V2, Verts))) {
      I_count++;
    } else if ('d' == op && (valid_verts(V1, V2, Verts))) {
      D_count++;
    }
  }
  fseek(file, 0, SEEK_SET);
}

void parse_update_file(
    FILE *&file, int *&E1, int *&E2, int *&DE1, int *&DE2,
    unsigned int &D_count, unsigned int &I_count, unsigned int &Verts,
    edge_set &deleted_edges) { // we will have a separate full
                               // array for addition of edges
  char op;
  unsigned int V1, V2;
  count_updates(file, D_count, I_count, Verts);
#if DEBUG
  printf("%d inserts and %d deletions\n", I_count, D_count);
#endif
  E1 = new int[I_count];
  E2 = new int[I_count];
  DE1 = new int[D_count];
  DE2 = new int[D_count];
  I_count = 0;
  D_count = 0;
  while (fscanf(file, "%c%*[ \t]%u%*[ \t]%u", &op, &V1, &V2) != EOF) {
    if ((valid_verts(V1, V2, Verts))) {
      if ('a' == op) {
#if DEBUG
        printf("seeing %c %u %u\n", op, V1, V2);
#endif
        E1[I_count] = V1;
        E2[I_count] = V2;
        I_count++;
      } else if ('d' == op) {
#if DEBUG
        printf("seeing %c %u %u\n", op, V1, V2);
#endif
        if (V1 < Verts / 2) {
          std::pair<int, int> temp = std::make_pair(V1, V2);
          deleted_edges.insert(temp);
          DE1[D_count] = V1;
          DE2[D_count] = V2;
          D_count++;
        } else {
          std::pair<int, int> temp = std::make_pair(V2, V1);
          deleted_edges.insert(temp);
          DE1[D_count] = V2;
          DE2[D_count] = V1;
          D_count++;
        }
      }
    }
  }
}

void make_diff_csr(int *&E1, int *&E2, unsigned int *&diff_edges,
                   unsigned int *&diff_deg_list, unsigned int &vertices,
                   unsigned int &edges) {
  unsigned int *temp_count = new unsigned int[vertices + 1];
  for (unsigned int i = 0; i < vertices + 1; i++) {
    temp_count[i] = 0;
  }
  for (unsigned int i = 0; i < edges; i++) {
    temp_count[E1[i]]++;
    temp_count[E2[i]]++;
  }
  for (unsigned int i = 0; i < vertices; i++) {
    diff_deg_list[i + 1] = diff_deg_list[i] + temp_count[i];
  }
  std::copy(diff_deg_list, diff_deg_list + vertices, temp_count);
  for (unsigned int i = 0; i < edges; i++) {
    diff_edges[temp_count[E1[i]]++] = E2[i];
    diff_edges[temp_count[E2[i]]++] = E1[i];
  }
  delete[] temp_count;
}

void graph_update(graph *&Bi_G, char *&file) {
  FILE *update_file;
  update_file = fopen(file, "r");
  int *E1, *E2;
  int *DE1, *DE2;
  unsigned int D_count = 0, I_count = 0;
  int *descendant, *end_points, *used;
  unsigned int *diff_edges, *diff_deg_list;
  edge_set deleted_edges;
#if DEBUG
  printf("Starting to make potential paths\n");
#endif
  make_potential_paths(Bi_G, descendant, end_points);

#if DEBUG
  printf("Parsing file for getting insertion and deletions\n");
#endif
  parse_update_file(update_file, E1, E2, DE1, DE2, D_count, I_count,
                    Bi_G->vertices, deleted_edges);
  fclose(update_file);
  printf("Starting with updating the graph\n");
  double start = omp_get_wtime();
  update_inserting_edges(Bi_G, descendant, end_points, E1, E2, I_count, used);
  diff_edges =
      new unsigned int[I_count *
                       2]; // for evey insertion, we need two extra spaces
  diff_deg_list = new unsigned int[Bi_G->vertices + 1];
  for (unsigned int i = 0; i < Bi_G->vertices + 1; i++) {
    diff_deg_list[i] = 0;
  }
  make_diff_csr(E1, E2, diff_edges, diff_deg_list, Bi_G->vertices, I_count);
  update_deleteing_edges(Bi_G, DE1, DE2, D_count);
  printf("Graph has been updated with new info, starting for bipartite "
         "matching now\n");
  M_match(Bi_G, diff_deg_list, diff_edges, deleted_edges);
  double end = omp_get_wtime();
  printf("\n%f second taken for update\n", end - start);
  delete[] E1;
  delete[] E2;
  delete[] DE1;
  delete[] DE2;
  delete[] descendant;
  delete[] used;
  delete[] end_points;
  delete[] diff_edges;
  delete[] diff_deg_list;
}
