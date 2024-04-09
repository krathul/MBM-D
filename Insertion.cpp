#include "Insertion.h"
#include <cstdio>
#define DEBUG 1

void search_path(unsigned int &vert, int *&descendant, graph *&Bi_G,
                 int *&ina_path, int *&curr, int *&next, int *&path,
                 int *&in_bfs, int *&end_points) {
#if DEBUG
  printf("Searching path for %d\n", vert);
#endif
  int end_vertex, bfs_count = 1, *temp_pointer;
  int pathfound = 0,
      new_path_found = 0; // to demarcate if we have clashing path or new path
  curr[0] = vert;
  in_bfs[vert] = 1;
  while (bfs_count) {
    bfs_count = 0;
    for (int i = 0; i < bfs_count; i++) {
      int curr_vert = curr[i];
      if (ina_path[curr_vert]) { // clashing path is found, hence we store it
                                 // temporarily
        pathfound = 1;
        end_vertex = end_points[curr_vert];
      } else {
        int neigh_count, *neighbours;
        neigh_count = Bi_G->in_degree_list[curr_vert + 1] -
                      Bi_G->in_degree_list[curr_vert];
        neighbours = &Bi_G->ins[Bi_G->in_degree_list[curr_vert]];
        for (int j = 0; j < neigh_count; j++) {
          int neighbour = neighbours[j];
          if (Bi_G->matching[neighbour] != -1 &&
              !in_bfs[Bi_G->matching[neighbour]]) { // if neighbour has a
                                                    // matching add matching to
                                                    // bfs array
            next[bfs_count++] = Bi_G->matching[neighbour];
            in_bfs[Bi_G->matching[neighbour]] = 1;
            path[Bi_G->matching[neighbour]] = curr_vert;
          } else { // neighbour is unmatched, end search_path
            pathfound = 1;
            new_path_found = 1;
            end_vertex = neighbour;
            break;
          }
        }
        neigh_count = Bi_G->out_degree_list[curr_vert + 1] -
                      Bi_G->out_degree_list[curr_vert];
        neighbours = &Bi_G->outs[Bi_G->out_degree_list[curr_vert]];
        for (int j = 0; j < neigh_count; j++) {
          int neighbour = neighbours[j];
          if (Bi_G->matching[neighbour] != -1 &&
              !in_bfs[Bi_G->matching[neighbour]]) { // if neighbour has a
                                                    // matching add matching to
                                                    // bfs array
            next[bfs_count++] = Bi_G->matching[neighbour];
            in_bfs[Bi_G->matching[neighbour]] = 1;
            path[Bi_G->matching[neighbour]] = curr_vert;
          } else { // neighbour is unmatched, end search_path
            pathfound = 1;
            new_path_found = 1;
            end_vertex = neighbour;
            break;
          }
        }
      }
    }
    temp_pointer = curr;
    curr = next;
    next = temp_pointer;
  }
  if (pathfound) {
    int temp = end_vertex;
    while (true) {
      if (temp == vert) {
        break;
      }
      descendant[path[temp]] = temp;
      temp = path[temp];
      ina_path[temp] = 1;   // marking parent belongs to a path now
      if (new_path_found) { // updating parents endpoints
        end_points[temp] = end_vertex;
      } else {
        end_points[temp] = end_points[end_vertex];
      }
    }
  }
#if DEBUG
  printf("%d's path found\n", vert);
#endif
}

void make_potential_paths(
    graph *&Bi_G, int *&descendant,
    int *&end_points) { // see if we can make an array to account for vertices
                        // already searched path for, in serial these vertices
                        // can be discarded after searched once tbh
  int *ina_path,
      *path; // path will allow reverse travel to update descendant
  int *bfs_verts, *next_bfs_verts, *in_bfs;
  unsigned int vertices = Bi_G->vertices;
  // potentially these below arrays can be made half their size
  path = new int[vertices];       // for noting down thw path
  descendant = new int[vertices]; //-1 means no descendant, or else points to
                                  // potential descendant that can be used
  end_points = new int[vertices]; // tell end point of the point of given vertex
  ina_path = new int[vertices];   // 1 means vertex is part of potential path, 0
                                  // implies not part of any path
  bfs_verts =
      new int[vertices]; // to make note of the path to make for a vertex
  in_bfs = new int[vertices];
  next_bfs_verts = new int[vertices];
  for (unsigned int i = 0; i < vertices; i++) {
    descendant[i] = -1;
    ina_path[i] = 0;
    in_bfs[i] = 0;
  }
  for (unsigned int i = 0; i < vertices / 2; i++) {
    for (int j = 0; j < vertices; j++) {
      path[j] = -1;
      in_bfs[j] = 0;
    }
    if (Bi_G->matching[i] != -1 && !ina_path[i]) {
      search_path(i, descendant, Bi_G, ina_path, bfs_verts, next_bfs_verts,
                  path, in_bfs, end_points);
    }
  }
#if DEBUG
  printf("Potential paths found\n");
#endif
  delete[] path;
  delete[] in_bfs;
  delete[] ina_path;
  delete[] bfs_verts;
  delete[] next_bfs_verts;
#if DEBUG
  printf("Potential paths found\n");
#endif
}

void update_because_path_exists(
    int *&descendant, int *&end_points, graph *&Bi_G,
    int V) { // passing V not by reference since we are updating it in function
#if DEBUG
  printf("%d being updated because a path is available\n", V);
#endif
  while (descendant[V] != -1) { // stop when no more descendant is there
    Bi_G->matching[V] = Bi_G->matching[descendant[V]];
    Bi_G->matching[descendant[V]] = V;
    V = descendant[V];
  }
  Bi_G->matching[V] = end_points[V];
  Bi_G->matching[end_points[V]] = V;
}

void update_inserting_edges(graph *&Bi_G, int *&descendant, int *&end_points,
                            int *&E1, int *&E2, unsigned int &I_count,
                            int *&used) {
  int *matching = Bi_G->matching;
#if DEBUG
  printf("%d vertices\n", Bi_G->vertices);
#endif
  used = new int[Bi_G->vertices]; // tells if that endpoint was already
                                  // used to find as a path
  for (unsigned int i = 0; i < Bi_G->vertices; i++) {
    used[i] = 0;
  }
  for (unsigned int i = 0; i < I_count; i++) {
    int V1 = E1[i];
    int V2 = E2[i];
    if (V1 > V2) {
      std::swap(V1, V2); // ensure V1 is smaller i.e, V1<N/2
    }
#if DEBUG
    printf("checking %d<-->%d\n", V1, V2);
#endif
    if (matching[V1] == -1 && matching[V2] == -1) {
#if DEBUG
      printf("adding %d--%d\n", V1, V2);
#endif
      matching[V1] = V2;
      matching[V2] = V1;
      Bi_G->cardinality++;
      continue;
    } else if (matching[V1] != -1 &&
               matching[V2] == -1) { // current implementation, doesnt have path
                                     // for vertices > N/2
      continue;
    } else if (matching[V1] == -1 && !used[end_points[matching[V2]]]) {
      // do it atomic if implemented in parallel
      used[end_points[matching[V2]]] = 1;
      update_because_path_exists(descendant, end_points, Bi_G, matching[V2]);
      matching[V2] = V1;
      matching[V1] = V2;
      Bi_G->cardinality++;
    }
  }
}
