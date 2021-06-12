vpath %.cpp src
vpath %.cu src
vpath %.hpp include

CC = nvcc
CPPFLAGS = -g -O3 -Xcompiler -fopenmp \

main : main.o mat.o
	$(CC) $(CPPFLAGS) -o main main.o mat.o

main.o : main.cpp
	$(CC) $(CPPFLAGS) -c src/main.cpp

mat.o : mat.cu mat.hpp
	$(CC) $(CPPFLAGS) -arch=sm_70 --expt-relaxed-constexpr -Xptxas -v -c src/mat.cu
	$(CC) $(CPPFLAGS) -arch=sm_70 --expt-relaxed-constexpr -ptx -Xptxas -v -c src/mat.cu

distclean :
	-rm main *.o

clean :
	-rm *.o
