CXX = nvcc
flags = -O3 -g -lcublas -Xcompiler -fopenmp

vpath %.cuh include
vpath %.cu src
vpath %.hpp include
vpath %.cpp src

source = src/main.cu src/conv.cu src/helper.cpp
header = include/conv.cuh include/helper.hpp
objects = main.o conv.o helper.o
target = main

$(target) : $(objects)
	$(CXX) -o $(target) $(objects) $(flags)

helper.o : helper.cpp
	$(CXX) -o helper.o -c src/helper.cpp $(flags)

conv.o : conv.cu
	$(CXX) -o conv.o -c src/conv.cu $(flags)

main.o : main.cu
	$(CXX) -o main.o -c src/main.cu $(flags)

clean :
	-rm $(target) $(objects)

cleanobj : 
	-rm $(objects)