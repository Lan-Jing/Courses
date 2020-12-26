#!/bin/bash

printf "\nMaking parallelization library.\n"
make -f Make_parallel_for clean
make -f Make_parallel_for 

printf "\nMaking test program.\n"
make -f Make_paramul clean
make -f Make_paramul option=-DDEBUG

export LD_LIBRARY_PATH=./:$LD_LIBRARY_PATH
./parallelized_matmul