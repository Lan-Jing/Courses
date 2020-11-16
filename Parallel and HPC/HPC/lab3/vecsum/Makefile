CC = gcc

objects = pthread_vecsum.o
target = pthread_vecsum

flags = $(option)
flags += -g -O3 -lpthread -fopenmp

$(target) : $(objects)
	gcc -o pthread_vecsum pthread_vecsum.c $(flags)

clean : 
	-rm $(target) $(objects)

help :
	@echo "Options:"
	@echo "	-DDEBUG: enable result check."
	@echo "	-DblcokSum: switch to block method"