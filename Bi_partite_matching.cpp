#include <cstdio>

#define V 0
#define DEBUG 0
#define LOGICAL_CHECK 0

#include "Bi_partite_matching.h"

void BFS(int &bfs_level, int *&bfs_array, graph *&g, int *&rmatch,
         int &augumenting_path_found, int &vertex_inserted, int *&predecessor,
         int &L0) {
  for (unsigned int i = 0; i < g->vertices / 2; i++) {
    if (bfs_level == bfs_array[i]) { // probably better to make a array with
                                     // only
      int degree = g->out_degree_list[i + 1] - g->out_degree_list[i];
      int *verts = &g->outs[g->out_degree_list[i]];
      for (int j = 0; j < degree; j++) {
        int neighbour = verts[j];
        int col_match = rmatch[neighbour];
#if DEBUG
        printf("vertex = %d, neightbour = %d\n", i, neighbour);
        printf("neighbour match is %d\n", col_match);
#endif
        if (col_match > -1) {
          if (bfs_array[col_match] == L0 - 1) {
#if DEBUG
            printf("doing new bfs level\n");
#endif
            vertex_inserted = 1;
            bfs_array[col_match] = bfs_level + 1;
#if LOGICAL_CHECK
            printf("making %d predecessor as %d\n", neighbour, col_match);
#endif
            predecessor[neighbour] = i;
          }
        } else if (col_match == -1) {
#if LOGICAL_CHECK
          printf("marking %d's rmatch as -2\n", neighbour);
          printf("making %d predecessor as %d\n", neighbour, i);
#endif
          rmatch[neighbour] = -2;
          predecessor[neighbour] = i;
#if DEBUG
          printf("Found augumenting path found\n");
#endif
          augumenting_path_found = 1;
        }
      }
      // do it again to emulate undirected, will add proper solo edgelist later
      degree = g->in_degree_list[i + 1] - g->in_degree_list[i];
      verts = &g->ins[g->in_degree_list[i]];
      for (int j = 0; j < degree; j++) {
        int neighbour = verts[j];
        int col_match = rmatch[neighbour];
#if DEBUG
        printf("vertex = %d, neightbour = %d\n", i, neighbour);
        printf("neighbour match is %d\n", col_match);
#endif
        if (col_match > -1) {
          if (bfs_array[col_match] == L0 - 1) {
#if DEBUG
            printf("doing new bfs level\n");
#endif
            vertex_inserted = 1;
            bfs_array[col_match] = bfs_level + 1;
#if LOGICAL_CHECK
            printf("making %d predecessor as %d\n", neighbour, col_match);
#endif
            predecessor[neighbour] = i;
          }
        } else if (col_match == -1) {
#if LOGICAL_CHECK
          printf("marking %d's rmatch as -2\n", neighbour);
          printf("making %d predecessor as %d\n", neighbour, i);
#endif
          rmatch[neighbour] = -2;
          predecessor[neighbour] = i;
#if DEBUG
          printf("Found augumenting path found\n");
#endif
          augumenting_path_found = 1;
        }
      }
    }
  }
}

void alternate(int *&cmatch, int *&rmatch, int *&predecessor, graph *&g) {
  int vertices = g->vertices;
  for (int i = vertices / 2; i < vertices; i++) {
    int r = i;
    if (rmatch[r] == -2) {
#if DEBUG
      printf("\n\nupdating path whose free vertex is %d\n", i);
#endif
#if LOGICAL_CHECK
      printf("%d's match is %d\n", r, rmatch[r]);
#endif
      while (r != -1) {
        int mc = predecessor[r];
        if (mc < 0) {
          break;
        }
        int mr = cmatch[mc];
#if LOGICAL_CHECK
        printf("%d is predecessor of %d but has %d as its match\n", mc, r, mr);
        printf("checking if %d is valid path to update\n", r);
        printf("mr=%d, and its predecessor is %d\n", mr, predecessor[mr]);
#endif
        if (mr >= 0 && predecessor[mr] == mc) {
          break;
        }
#if LOGICAL_CHECK
        printf("new match %d<->%d\n", mc, r);
#endif
        cmatch[mc] = r;
        rmatch[r] = mc;
        r = mr;
      }
    }
  }
}

void fixmatching(int *&cmatch, int *&rmatch, graph *&g) {
  int vertices = g->vertices;
  for (int i = vertices / 2; i < vertices; i++) {
    if ((rmatch[i] == -2) ||
        (rmatch[i] >= 0 &&
         cmatch[rmatch[i]] !=
             i)) { // couldnt find its match or has been wrongly matched
#if LOGICAL_CHECK
      printf("marking back %d's rmatch as -1\n", i);
#endif
      rmatch[i] = -1;
    }
  }
}

void match(graph *&Bi_G) {
  unsigned vertices = Bi_G->vertices;
  int L0 = 0;
  //////////////////////////////////////
  int *cmatch = new int[vertices];
  int *rmatch = new int[vertices];
  int *predecessor = new int[vertices];
  //////////////////////////////////////
  for (unsigned int i = 0; i < vertices; i++) {
    cmatch[i] = -1;
    rmatch[i] = -1;
  }
  //////////////////////////////////////
  int *bfs_array = new int[vertices];
  int augumenting_path_found = 1, vertex_inserted;
  int bfs_level;
  //////////////////////////////////////
  while (augumenting_path_found) {
    for (unsigned int i = 0; i < vertices; i++) {
      predecessor[i] = -1;
    }
    augumenting_path_found = 0;
    bfs_level = L0;
    vertex_inserted = 1;
    for (unsigned int i = 0; i < vertices / 2;
         i++) { // taking 0...n/2-1 as the vertices
                // in consideration under one set
      if (cmatch[i] > -1) {
        bfs_array[i] = L0 - 1;
      } else if (cmatch[i] == -1) {
        bfs_array[i] = L0;
      }
    }
#if DEBUG
    printf("\n\nstarting new bfs to find a new path\n");
#endif

    while (vertex_inserted) {
#if DEBUG
      printf("%d level bfs\n", bfs_level);
#endif
      vertex_inserted = 0;
      BFS(bfs_level, bfs_array, Bi_G, rmatch, augumenting_path_found,
          vertex_inserted, predecessor, L0);
      if (augumenting_path_found) {
#if DEBUG
        printf("path found once\n");
#endif
        break;
      }
      bfs_level += 1;
    }
    alternate(cmatch, rmatch, predecessor, Bi_G);
    fixmatching(cmatch, rmatch, Bi_G);
  }
  delete[] predecessor;
  delete[] bfs_array;
  int *matching = Bi_G->matching;
  Bi_G->cardinality = 0;
  for (unsigned int i = 0; i < vertices / 2; i++) {
    matching[i] = cmatch[i];
    if (cmatch[i] != -1) {
#if LOGICAL_CHECK
      printf("%d has a match %d\n", i, cmatch[i]);
#endif
      Bi_G->cardinality++; // add 1 only in this half
    }
  }
  for (unsigned int i = vertices / 2; i < vertices; i++) {
    matching[i] = rmatch[i];
  }
  delete[] cmatch;
  delete[] rmatch;
}
