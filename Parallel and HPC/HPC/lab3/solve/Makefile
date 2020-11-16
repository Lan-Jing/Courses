CC = gcc

objects = pthread_solve.o
target = pthread_solve

flags = -g -O3 -lpthread

$(target) : $(objects)
	$(CC) -o $(target) pthread_solve.c $(flags)

clean :
	-rm $(objects) $(target)