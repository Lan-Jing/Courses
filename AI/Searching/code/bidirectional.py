import numpy as np
import queue as qe
import data

gox = (-1, 1, 0, 0)
goy = (0, 0, 1, -1)

def fetch_node(queue):
    node = queue.get()
    loc = node[-1]
    return node, loc

# construct path given the convergence point
def construct_path(fmap, bmap, loc):
    # the meeting point will be appended twice. Make it unique
    res = fmap[loc][0:-1]
    bmap[loc].reverse()
    res.extend(bmap[loc])
    return res

# bidirectional bfs
def bd_search(graph):
    sloc = data.locate_point(graph, 'S')
    eloc = data.locate_point(graph, 'E')
    fqueue = qe.Queue(maxsize=0)
    fmap = {}
    bqueue = qe.Queue(maxsize=0)
    bmap = {}
    visited = np.zeros(np.shape(graph), dtype=int)

    fqueue.put([sloc])
    bqueue.put([eloc])
    while fqueue.empty() == 0 or bqueue.empty() == 0:
        node, loc = fetch_node(fqueue)
        while visited[loc[0]][loc[1]] == 1 and fqueue.empty() == 0:
            node, loc = fetch_node(fqueue)

        if visited[loc[0]][loc[1]] % 2 == 0:
            visited[loc[0]][loc[1]] += 1
            fmap[loc] = node

            if visited[loc[0]][loc[1]] == 3:
                return construct_path(fmap, bmap, loc)
            else:
                newLoc = list(loc)
                for i in range(4):
                    newLoc[0] = loc[0] + goy[i]
                    newLoc[1] = loc[1] + gox[i]
                    if graph[newLoc[0]][newLoc[1]] == '1':
                        continue
                    newNode = node.copy()
                    newNode.append(tuple(newLoc))
                    fqueue.put(newNode)

        node, loc = fetch_node(bqueue)
        while visited[loc[0]][loc[1]] == 2 and bqueue.empty() == 0:
            node, loc = fetch_node(bqueue)

        if visited[loc[0]][loc[1]] <= 1:
            visited[loc[0]][loc[1]] += 2
            bmap[loc] = node

            if visited[loc[0]][loc[1]] == 3:
                return construct_path(fmap, bmap, loc)
            else:
                newLoc = list(loc)
                for i in range(4):
                    newLoc[0] = loc[0] + goy[i]
                    newLoc[1] = loc[1] + gox[i]
                    if graph[newLoc[0]][newLoc[1]] == '1':
                        continue
                    newNode = node.copy()
                    newNode.append(tuple(newLoc))
                    bqueue.put(newNode)
    
    return []