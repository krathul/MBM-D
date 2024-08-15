import sys
import subprocess
import os

if __name__ == '__main__':
    edges = {}
    u = {}
    v = {}
    u_cnt = 0
    v_cnt = 0
    edge_cnt = 0

    save_file = open(sys.argv[1].split('.')[0], "w")

    with open(sys.argv[1]) as input, open("test/temp1", "w") as output:
        for line in input:
            if '#' in line:
                continue

            line_entry = line.strip().split()
            node1 = line_entry[0]
            node2 = line_entry[1]

            if node1 not in u.keys():
                u_cnt += 1
                u[node1] = u_cnt - 1
            if node2 not in v.keys():
                v_cnt += 1
                v[node2] = v_cnt - 1

            if (u[node1], v[node2]) in edges.keys():
                continue
            else:
                edges[(u[node1], v[node2])] = True
                edge_cnt += 1
                output.write(str(u[node1]) + "\t" + str(v[node2])
                             + "\n")

        save_file.write("#" + str(u_cnt) + "," + str(v_cnt) + "," +
                        str(edge_cnt) + "\n")

    vertices = u_cnt + v_cnt
    with open("test/temp1", "r") as output:
        for line in output:
            u, v = map(int, line.strip().split())
            save_file.write(str(u) + "\t" + str(v+u_cnt) + "\n")

    os.remove("test/temp1")
    save_file.close()
