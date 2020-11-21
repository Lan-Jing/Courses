#%%
import numpy

import data
import bidirectional as bd
import IDA

def run_maze(search_func, graph):
    path = search_func(graph)
    if len(path) == 0:
        print("Maze unsolvable.\n")
    else:
        print("Shortest Path with {} steps:\n".format(len(path)-1), path)

def main():
    graph = data.readin("../data/MazeData.txt")
    run_maze(bd.bd_search, graph)
    run_maze(IDA.IDA_search, graph)

if __name__ == "__main__":
    main()
# %%
