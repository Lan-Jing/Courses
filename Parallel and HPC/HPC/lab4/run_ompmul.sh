#!/bin/bash

make -f Make_ompmul clean
make -f Make_ompmul option=-DDEBUG

export OMP_NUM_THREADS=$1
echo "Number of threads set as $OMP_NUM_THREADS"
export OMP_SCHEDULE="$2"
echo "Scheduling strategy set as $OMP_SCHEDULE"
./openmp_matmul