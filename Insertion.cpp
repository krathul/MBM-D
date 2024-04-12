#include "Insertion.h"
#include <cstdio>
#include <set>
#define DEBUG 0
#define LOGICAL_ERROR 0
#define PROGRESS_CHECK 0

int threshold = 1;

typedef std::set<int> new_set;
int in_set(new_set &to_check, int &vert) {
  if (to_check.find(vert) != to_check.end()) {
    return 1;
  } else {
    return 0;
  }
}

void traverse_neighbours(int &curr_vert, int *&degree_list, int *&edges,
                         int *&matching, int *&next, int *&path, int &bfs_count,
                         int &pathfound, int &new_path_found, int &end_vertex,
                         int &path_end, new_set &bfs_set) {
  int neigh_count, *neighbours;
  neigh_count = degree_list[curr_vert + 1] - degree_list[curr_vert];
  neighbours = &edges[degree_list[curr_vert]];
  for (int j = 0; j < neigh_count; j++) {
    int neighbour = neighbours[j];
    int nmatch = matching[neighbour];
#if LOGICAL_ERROR
    printf("checking neighbour %d, nmatch is %d and bfs status %d\n", neighbour,
           nmatch, in_bfs[nmatch]);
#endif
    if (nmatch != -1 && !in_set(bfs_set, nmatch)) { // if neighbour has a
                                                    // matching add matching to
                                                    // bfs array
      next[bfs_count++] = nmatch;
      // in_bfs[nmatch] = 1;
      bfs_set.insert(nmatch);
      path[nmatch] = curr_vert;
    } else if (nmatch == -1) { // neighbour is unnmatched, end search_path
#if LOGICAL_ERROR
      printf("new path found, path end in %d with end vertex %d, will stop "
             "exploring neighbours\n",
             curr_vert, neighbour);
#endif
      pathfound = 1;
      new_path_found = 1;
      end_vertex = neighbour;
      path_end = curr_vert;
      break;
    }
  }
}

void search_path(unsigned int &vert, int *&descendant, graph *&Bi_G,
                 int *&ina_path, int *&curr, int *&next, int *&path,
                 int *&end_points) {
  new_set bfs_set;
#if LOGICAL_ERROR
  printf("Searching path for %d\n", vert);
#endif
  int end_vertex, path_end, bfs_count = 1, next_bfs_count,
                            *temp_pointer; // path end is paths end, end_vertex
                                           // is the potential match we go with
  int pathfound = 0,
      new_path_found = 0; // to demarcate if we have clashing path or new path
  curr[0] = vert;
  while (bfs_count) {
#if LOGICAL_ERROR
    printf("BFSing\n");
#endif
    next_bfs_count = 0;
    for (int i = 0; i < bfs_count; i++) {
      int curr_vert = curr[i];
      if (ina_path[curr_vert]) { // clashing path is found, hence we store it
                                 // temporarily
        pathfound = 1;
        path_end = curr_vert;
        end_vertex = end_points[curr_vert];
      } else {
        traverse_neighbours(curr_vert, Bi_G->in_degree_list, Bi_G->ins,
                            Bi_G->matching, next, path, next_bfs_count,
                            pathfound, new_path_found, end_vertex, path_end,
                            bfs_set);
        if (new_path_found) {
          break;
        }
        traverse_neighbours(curr_vert, Bi_G->out_degree_list, Bi_G->outs,
                            Bi_G->matching, next, path, next_bfs_count,
                            pathfound, new_path_found, end_vertex, path_end,
                            bfs_set);
        if (new_path_found) {
          break;
        }
      }
    }
    bfs_count = next_bfs_count;
    temp_pointer = curr;
    curr = next;
    next = temp_pointer;
  }
#if LOGICAL_ERROR
  if (new_path_found) {
    printf("new path found for %d with %d end_point\n", vert, end_vertex);
  }
#endif
  if (pathfound) {
    int temp = path_end;
    while (true) {
#if LOGICAL_ERROR
      printf("Updating descendants\n");
#endif
      ina_path[temp] = 1;
      if (new_path_found) { // updating parents endpoints
        end_points[temp] = end_vertex;
      } else {
        end_points[temp] = end_points[path_end];
      }
      if (temp == vert) {
        break;
      }
      descendant[path[temp]] = temp;
      temp = path[temp];
    }
#if LOGICAL_ERROR
    printf("%d's path found with end vertex as %d\n", vert, end_points[vert]);
#endif
  }
}

void make_potential_paths(
    graph *&Bi_G, int *&descendant,
    int *&end_points) { // see if we can make an array to account for vertices
                        // already searched path for, in serial these vertices
                        // can be discarded after searched once tbh
  int *ina_path,
      *path; // path will allow reverse travel to update descendant
  int *bfs_verts, *next_bfs_verts;
  unsigned int vertices = Bi_G->vertices;
  // potentially these below arrays can be made half their size
  path = new int[vertices];       // for noting down thw path
  descendant = new int[vertices]; //-1 means no descendant, or else points to
                                  // potential descendant that can be used
  end_points = new int[vertices]; // tell end point of the point of given
                                  // vertex, -1 means no path found
  ina_path = new int[vertices];   // 1 means vertex is part of potential path, 0
                                  // implies not part of any path
  bfs_verts =
      new int[vertices]; // to make note of the path to make for a vertex
  next_bfs_verts = new int[vertices];
  for (unsigned int i = 0; i < vertices; i++) {
    end_points[i] = -1;
    descendant[i] = -1;
    ina_path[i] = 0;
  }
  for (unsigned int i = 0; i < vertices / 2; i++) {
#if PROGRESS_CHECK
    if (i == vertices * threshold / 200) {
      printf("%d%% done\n", threshold);
      threshold *= 2;
    }
#endif
    if (Bi_G->matching[i] != -1 && !ina_path[i]) {
      search_path(i, descendant, Bi_G, ina_path, bfs_verts, next_bfs_verts,
                  path, end_points);
    }
  }
  delete[] path;
  delete[] ina_path;
  delete[] bfs_verts;
  delete[] next_bfs_verts;
#if DEBUG
  printf("Potential paths found\n");
#endif
  return;
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
    int path_exists = 0;
    if (matching[V2] >= 0) {
      if (end_points[matching[V2]] >= 0) {
        if (!used[end_points[matching[V2]]]) {
          // do it atomically in parallel
          used[end_points[matching[V2]]] = 1;
          path_exists = 1;
        }
      }
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
    } else if (matching[V1] != -1 && matching[V2] == -1) {
      continue; // current implementation, doesnt have path for vertices > N/2
    } else if (matching[V1] == -1 && path_exists) {
      update_because_path_exists(descendant, end_points, Bi_G, matching[V2]);
      matching[V2] = V1;
      matching[V1] = V2;
      Bi_G->cardinality++;
    }
  }
}
