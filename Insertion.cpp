#include "Insertion.h"

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
  while (bfs_count) {
    bfs_count = 0;
    for (int i = 0; i < bfs_count; i++) {
      int curr_vert = curr[i];
      if (ina_path[curr_vert]) { // clashing path is found, hence we store it
                                 // temporarily
        pathfound = 1;
        end_vertex = curr_vert;
      } else {
        int neigh_count, *neighbours;
        neigh_count = Bi_G->in_degree_list[curr_vert + 1] -
                      Bi_G->in_degree_list[curr_vert];
        neighbours = &Bi_G->ins[Bi_G->in_degree_list[curr_vert]];
        for (int j = 0; j < neigh_count; j++) {
          int neighbour = neighbours[j];
          if (Bi_G->matching[neighbour] !=
              -1) { // if neighbour has a matching add matching to bfs array
            next[bfs_count++] = Bi_G->matching[neighbour];
            path[Bi_G->matching[neighbour]] = curr_vert;
          } else { // neighbour is unmatched, end search_path
            pathfound = 1;
            new_path_found = 1;
            end_vertex = curr_vert;
            break;
          }
        }
        neigh_count = Bi_G->out_degree_list[curr_vert + 1] -
                      Bi_G->out_degree_list[curr_vert];
        neighbours = &Bi_G->outs[Bi_G->out_degree_list[curr_vert]];
        for (int j = 0; j < neigh_count; j++) {
          int neighbour = neighbours[j];
          if (Bi_G->matching[neighbour] !=
              -1) { // if neighbour has a matching add matching to bfs array
            next[bfs_count++] = Bi_G->matching[neighbour];
            path[Bi_G->matching[neighbour]] = curr_vert;
          } else { // neighbour is unmatched, end search_path
            pathfound = 1;
            new_path_found = 1;
            end_vertex = curr_vert;
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
      ina_path[temp] = 1; // marking parent belongs to a path now
    }
    if (new_path_found) {
      end_points[vert] = end_vertex;
    } else {
      end_points[vert] = end_points[end_vertex];
    }
  }
}

void make_potential_paths(graph *&Bi_G, int *&descendant, int *&end_points) {
  int *ina_path,
      *path; // path will allow reverse travel to update descendant
  int *bfs_verts, *next_bfs_verts, *in_bfs;
  unsigned int vertices = Bi_G->vertices;
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
    if (Bi_G->matching[i] != -1) {
      search_path(i, descendant, Bi_G, ina_path, bfs_verts, next_bfs_verts,
                  path, in_bfs, end_points);
    }
  }
  delete[] path;
  delete[] ina_path;
  delete[] bfs_verts;
  delete[] next_bfs_verts;
}

void update_inserting_edges(graph *&Bi_G) {}
