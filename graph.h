#include <string>

struct graph {
  int vertices;
  int edges;
  int *ins;
  int *outs;
  int *in_degree_list;
  int *out_degree_list;
};

typedef struct graph graph;

graph *make_graph(FILE *file, int verts);

void clean_graph(graph *g);
