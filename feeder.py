import sys
import math
import subprocess
import networkx as nx


def in_helper(batches, u_verts, v_verts):
    d_log = open("log/d_in", "w")
    s_log = open("log/s_in", "w")

    d_log.write("test/input %s %s test/update1\n" %
                (str(u_verts), str(v_verts)))
    for batch in range(batches):
        s_log.write("test/input%d %s %s" %
                    (batch+1, str(u_verts), str(v_verts)) + "\n")

    for batch in range(1, batches):
        d_log.write("test/input%d %s %s test/update%d\n" %
                    (batch, str(u_verts), str(v_verts), batch + 1))


def splitter2(update_file, edges, u_verts, v_verts, inc=0.02):
    update_file.seek(0)
    batches = int(1/inc)
    for i in range(batches):
        subprocess.run(["cp", "test/input", "test/input%d" % (i+1)])

    for batch in range(batches):
        with open("test/update%d" % (batch+1), "w") as batch_file:
            for i in range(math.floor(batch*inc*edges), math.floor((batch+1)*inc*edges)):
                line = update_file.readline()
                batch_file.write("a " + line)
                for j in range(batch, batches):
                    with open("test/input%d" % (j+1), "a") as input_file:
                        input_file.write(line)

    in_helper(batches, u_verts, v_verts)


def splitter():
    dataset = sys.argv[1]
    split = 0.2

    input = open("test/input", "w")
    empty = open("test/empty", "w")
    empty.close()
    update = open("test/update", "w+")

    with open(dataset) as data:
        u, v, edges = map(int, data.readline()[1:].split(','))

        for i in range(math.floor(edges*split)):
            input.write(data.readline())
        for rem_line in data:
            update.write(rem_line)

    input.close()
    splitter2(update, edges-math.floor(edges*split), u, v)
    update.close()
    return u+v


print(splitter())
