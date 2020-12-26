CXX = nvcc
flags = -O3 -lcublas -Xcompiler -fopenmp

vpath %.h include
vpath %.cu src

source = src/main.cu src/matmul.cu src/helper.cu
header = include/matmul.cuh include/helper.h
objects = main.o matmul.o helper.o
target = main

$(target) : $(objects)
	$(CXX) -o $(target) $(objects) $(flags)

helper.o : helper.cu
	$(CXX) -o helper.o -c src/helper.cu $(flags)

matmul.o : matmul.cu
	$(CXX) -o matmul.o -c src/matmul.cu $(flags)

main.o : main.cu
	$(CXX) -o main.o -c src/main.cu $(flags)

clean :
	-rm $(target) $(objects)

cleanobj :
	-rm $(objects)