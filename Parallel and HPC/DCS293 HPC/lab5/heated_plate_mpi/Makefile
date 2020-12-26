CC = mpicc
flags = -g -Ofast -fopenmp

vpath %.h include
vpath %.c src

source = src/heated_plate_MPI.c src/comm_MPI.c
objects = heated_plate_MPI.o comm_MPI.o
target = heated_plate_MPI

$(target) : $(objects)
	$(CC) -o $(target) $(objects) $(flags)

heated_plate_MPI.o : heated_plate_MPI.c
	$(CC) -o heated_plate_MPI.o -c src/heated_plate_MPI.c $(flags)

comm_MPI.o : comm_MPI.c
	$(CC) -o comm_MPI.o -c src/comm_MPI.c $(flags)

clean :
	-rm $(target) $(objects)

cleanobj :
	-rm $(objects)