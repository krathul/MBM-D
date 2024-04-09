#include "graph.h"

void update_inserting_edges(graph *&Bi_G, int *&descendant, int *&end_points,
                            int *&E1, int *&E2, unsigned int &I_count,
                            int *&used);

void make_potential_paths(graph *&Bi_G, int *&descendant, int *&end_points);

void update_because_path_exists(int *&descendant, int *&end_points,
                                graph *&Bi_G, int V);
