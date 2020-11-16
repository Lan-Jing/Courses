CC = gcc

objects = pthread_matmul.o
target = pthread_matmul

flags = $(option)
flags += -g -O3 -lpthread -fopenmp

$(target) : $(objects)
	gcc -o pthread_matmul pthread_matmul.c mat.c $(flags)

clean :
	-rm $(target) $(objects)

help :
	@echo "Add -DDEBUG to enable result check."