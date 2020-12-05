#! /bin/bash

spack load valgrind

valgrind --tool=massif --massif-out-file=result/out ./run.sh $1
ms_print result/out > result/out_graph