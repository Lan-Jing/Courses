#! /bin/bash
#
make -f Make_parallel_for 
make -f Make_parallel_for cleanobj
printf "Pthread parallelization lib compiled.\n\n"

make -f Make_heated_plate
make -f Make_heated_plate cleanobj
printf "Heated Plate application compiled.\n"

export LD_LIBRARY_PATH=./:$LD_LIBRARY_PATH
./heated_plate $1