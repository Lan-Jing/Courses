#!/bin/bash
module load opt/openmpi/3.1.3-intel2018.4

make
make cleanobj
mpirun -np $1 ./heated_plate_MPI