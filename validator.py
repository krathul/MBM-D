import networkx as nx
from networkx.algorithms import bipartite
import sys

global graph


def build_graph(initial_file_path):
    global graph
    graph = nx.DiGraph()
    with open(initial_file_path, 'r') as file:
        for line in file:
            node1, node2 = map(int, line.strip().split())
            graph.add_edge(node1, node2)
    return graph


if __name__ == '__main__':
    input_file = sys.argv[1]
    update_file = sys.argv[2]
    with open(input_file) as input:
        for line in input:
            src, dst = line.strip().split()
