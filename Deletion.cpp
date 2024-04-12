#include "Deletion.h"
#include "Insertion.h"
/*
void update_deleteing_edges(graph *&Bi_G, int *&DE1, int *&DE2,
                            unsigned int &D_count, int *&descendant,
                            int *&end_points, int *&used) {
  for (unsigned int i = 0; i < D_count; i++) {
    int V1 = DE1[i];
    int V2 = DE2[i];
    if (V1 > V2) { // V1 should be in first half
      std::swap(V1, V2);
    }
    int *matching = Bi_G->matching;
    if (matching[V1] == V2) {
      if (!used[end_points[V1]]) {
        // should be atomic
        used[end_points[V1]] = 1;
        update_because_path_exists(descendant, end_points, Bi_G, V1);
      } else {
        matching[V1] = -1;
        Bi_G->cardinality--;
      }
      matching[V2] = -1;
    }
  }
}
*/

void update_deleteing_edges(graph *&Bi_G, int *&DE1, int *&DE2,
                            unsigned int &D_count) {
  int *matching = Bi_G->matching;
  for (unsigned int i = 0; i < D_count; i++) {
    if (matching[DE1[i]] == DE2[i]) {
      matching[DE1[i]] = -1;
      matching[DE2[i]] = -1;
      Bi_G->cardinality--;
    }
  }
}
