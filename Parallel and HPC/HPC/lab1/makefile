objects = main.o MatrixMul.o MatrixOp.o

vpath %.h include
vpath %.c src

# add -fopenmp flags into all g++/gcc invocation
main : $(objects)
	g++ -o main -fopenmp -msse -mavx -mavx2 $(objects)

main.o : MatrixOp.h
	g++ -c -g -fopenmp -msse -mavx -mavx2 -O3 src/main.c

MatrixMul.o : MatrixOp.h
	g++ -c -g -fopenmp -msse -mavx -mavx2 -O3 src/MatrixMul.c

MatrixOp.o : MatrixOp.h
	g++ -c -g -fopenmp -msse -mavx -mavx2 -O3 src/MatrixOp.c

# use command del for delete instead of rm in Windows
clean:
	-rm $(objects) main