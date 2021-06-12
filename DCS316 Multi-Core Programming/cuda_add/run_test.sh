#!/bin/bash

rm output/*

for i in 1 2 4 8
do
	sed -i "s/WORKLOAD [0-9]*/WORKLOAD $i/" include/add.hpp
	for j in 4 8 16 32
	do
		sed -i "s/BLOCKSIZE [0-9]*/BLOCKSIZE $j/" include/add.hpp
		make >> output/make.log
		for k in 1024 2048 4096 8192 16384
		do
			./add.exe $k 20 >> output/out-$k.log
		done
	done
done