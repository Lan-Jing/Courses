import numpy as np

def readin(path):
    graph = []
    with open(file=path, mode='r') as fp:
        for line in fp.readlines():
            graph.append(tuple(line.strip('\n')))
    graph = np.asarray(graph)
    print("Loaded graph:\n", graph)
    return graph

def locate_point(graph, char):
    for i in range(np.shape(graph)[0]):
        for j in range(np.shape(graph)[1]):
            if graph[i][j] != char:
                continue
            return i, j