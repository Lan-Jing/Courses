import numpy as np
import data

gox = (-1, 1, 0, 0)
goy = (0, 0, 1, -1)

def h(loca, locb):
    return abs(loca[0]-locb[0]) + abs(loca[1]-locb[1])

def fetch_node(queue):
    node = queue.get()
    loc = node[-1]
    return node, loc

resPath = []
resL = 0x3f3f3f3f

def IDA_dfs(graph, loc, eloc, bound, path, visited):
    global resL, resPath

    path.append(loc)
    visited[loc[0]][loc[1]] = len(path)-1
    if loc == eloc and len(path)-1 < resL:
        resL = len(path)-1
        resPath = path.copy()
        return 
    # A* pruning
    if h(loc, eloc) + len(path)-1 > bound:
        path.pop()
        return 

    newLoc = list(loc)
    for i in range(4):
        newLoc[0] = loc[0] + goy[i]
        newLoc[1] = loc[1] + gox[i]
        # pruning with shortest path recored by ''visited''
        if graph[newLoc[0]][newLoc[1]] == '1' or visited[newLoc[0]][newLoc[1]] <= len(path):
            continue
        IDA_dfs(graph, tuple(newLoc), eloc, bound, path, visited)
    
    path.pop()
    return 

def IDA_search(graph):
    sloc = data.locate_point(graph, 'S')
    eloc = data.locate_point(graph, 'E')

    global resPath

    for bound in range(h(sloc, eloc), np.shape(graph)[0] * np.shape(graph)[1]):
        visited = np.full(np.shape(graph), 0x3f3f3f3f, dtype=int)
        print('Bound: {}'.format(bound))
        IDA_dfs(graph, sloc, eloc, bound, [], visited)
        if len(resPath) == 0:
            continue
        return resPath

    return []