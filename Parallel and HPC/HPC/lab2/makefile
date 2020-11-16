vpath %.h include
vpath %.c src

cc = mpicc

objects = main.o MatrixMul.o MatrixOp.o mpi_comm.o
headers = MatrixOp.h mpi_comm.h
sources = main_scatter.c main_send.c

main = $(mode)
# endable DEBUG 
flags = $(option)
flags += -g -fopenmp -mavx -mavx2 -O3

# add -fopenmp flags into all g++/gcc invocation
main : $(objects)
	$(cc) -o main -fopenmp $(objects)

# choose send/recv or scatter/gather manner
main.o : $(headers) $(sources)
ifeq ($(main), scatter)
	$(cc) -o main.o -c $(flags) src/main_scatter.c
else 
	$(cc) -o main.o -c $(flags) src/main_send.c
endif

MatrixMul.o : $(headers) MatrixMul.c
	$(cc) -c $(flags) src/MatrixMul.c

MatrixOp.o : $(headers) MatrixOp.c
	$(cc) -c $(flags) src/MatrixOp.c

mpi_comm.o : $(headers) mpi_comm.c
	$(cc) -c $(flags) src/mpi_comm.c

# use command del for delete instead of rm in Windows
clean:
	-rm $(objects) main