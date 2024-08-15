import subprocess
import sys

if __name__ == '__main__':
    if (sys.argv[1] == 'static'):
        with open("log/s_in") as input:
            for line in input:
                graph_file, n_u, n_v = line.strip('\n').split(' ')
                subprocess.run(["./build/StaticMBM", graph_file, n_u, n_v])

    if (sys.argv[1] == 'dynamic'):
        with open("log/d_in") as input:
            for line in input:
                graph_file, n_u, n_v, update_file = line.strip('\n').split(' ')
                subprocess.run(
                    ["./build/DynamicMBM", graph_file, n_u, n_v, update_file])
