OBJS	= main.o vectormath.o ppmoutput.o
SOURCE	= main.c vectormath.c ppmoutput.c
HEADER	= vectormath.h ppmoutput.h
OUT	= raytracer
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lpthread -lm
PIC = result.ppm


all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c


vectormath.o: vectormath.c
	$(CC) $(FLAGS) vectormath.c

ppmoutput.o: ppmoutput.c
	$(CC) $(FLAGS) ppmoutput.c

clean:
	rm -f $(OBJS) $(OUT) $(PIC)

help:
		$(info Changing the number of threads that the code uses can be done by modifying the "threadnum" variable in main.c)
		$(info make clean - cleans the project folder)
		$(info Yes, I know I'm late with this thing and I'll say it again in the makefile.)
