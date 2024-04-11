import networkx as nx
from networkx.algorithms import bipartite
import sys


def read_file(filename, num_vertices):
    edges = []
    with open(filename, "r") as file:
        for line in file:
            a, b = map(int, line.strip().split())
            if 0 <= a < num_vertices // 2 and num_vertices // 2 <= b < num_vertices:
                temp = (a, b)
                edges.append(temp)
            elif 0 <= b < num_vertices // 2 and num_vertices // 2 <= a < num_vertices:
                temp = (b, a)
                edges.append(temp)
    print("has", len(edges), "edges")
    # for i in range(len(edges)):
    #   print(edges[i])
    return edges


def create_bipartite_graph(edges, num_vertices):
    G = nx.Graph()
    V1 = [i for i in range(0, num_vertices // 2)]
    V2 = [i for i in range(num_vertices // 2, num_vertices)]
    G.add_nodes_from(V1, bipartite=0)
    G.add_nodes_from(V2, bipartite=1)
    for a, b in edges:
        if 0 <= a < num_vertices // 2 and num_vertices // 2 <= b < num_vertices:
            temp = [(a, b)]
            G.add_edges_from(temp)
        elif 0 <= b < num_vertices // 2 and num_vertices // 2 <= a < num_vertices:
            temp = [(b, a)]
            G.add_edges_from(temp)
    print(G.nodes)
    print(bipartite.is_bipartite(G))
    return G


def maximum_matching(graph):
    u = [n for n in graph.nodes if graph.nodes[n]["bipartite"] == 0]
    print(u)
    matching = nx.bipartite.maximum_matching(graph, u)
    print(matching)
    return len(matching) // 2


def main():
    filename = sys.argv[1]
    num_vertices = int(sys.argv[2])
    edges = read_file(filename, num_vertices)
    # print("Edges read")
    # graph = create_bipartite_graph(edges, num_vertices)
    # print("graph made")
    # print("no of edges", len(graph.edges()))
    # max_cardinality = maximum_matching(graph)
    # print("\nMaximum cardinality of the matching:", max_cardinality)


main()
