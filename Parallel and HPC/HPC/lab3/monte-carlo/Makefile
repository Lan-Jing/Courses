CC = gcc

objects = pthread_monte-carlo.o
target = pthread_monte-carlo

flags = -g -O3 -lpthread -fopenmp

$(target) : $(objects)
	gcc -o pthread_monte-carlo pthread_monte-carlo.c $(flags)

clean :
	-rm $(target) $(objects)