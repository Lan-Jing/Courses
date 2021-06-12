vpath %.cu src
vpath %.cpp src
vpath %.hpp include

CXX = nvcc
flags = -O3 -Xcompiler -fopenmp \
	-Xptxas -O3 \
	-arch sm_70
target = add.exe

$(target) : main.o add.o helper.o
	$(CXX) -o $(target) main.o add.o helper.o $(flags)

main.o : main.cu add.hpp
	$(CXX) -o main.o -c src/main.cu $(flags)

add.o : add.cu add.hpp
	$(CXX) -o add.o -c src/add.cu $(flags)

helper.o : helper.cpp helper.hpp
	$(CXX) -o helper.o -c src/helper.cpp $(flags)

clean:
	-rm *.o $(target)

cleanobj:
	-rm *.o
