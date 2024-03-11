#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

#include "graph.h"

#define DEBUG 0

int main(int argc, char *argv[]) {
  cout << "First line" << endl;
  FILE *file;
  file = fopen(argv[1], "r");
  int verts = stoi(argv[2]);
  graph *Bi_G;
  Bi_G = make_graph(file, verts);
}
