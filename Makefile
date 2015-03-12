OBJS = point.o normal.o color.o vector.o BRDF.o light.o camera.o matrix.o shape.o main.o tests.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all: $(OBJS) 
	$(CC) $(LFLAGS) $(OBJS) -o main

point.o: point.h point.cpp
	$(CC) $(CFLAGS) point.cpp

normal.o: normal.h normal.cpp
	$(CC) $(CFLAGS) normal.cpp

color.o: color.h color.cpp
	$(CC) $(CFLAGS) color.cpp

vector.o: vector.h vector.cpp
	$(CC) $(CFLAGS) vector.cpp

BRDF.o: BRDF.h BRDF.cpp
	$(CC) $(CFLAGS) BRDF.cpp

light.o: light.h light.cpp
	$(CC) $(CFLAGS) light.cpp

camera.o: camera.h camera.cpp
	$(CC) $(CFLAGS) camera.cpp

matrix.o: matrix.h matrix.cpp
	$(CC) $(CFLAGS) matrix.cpp

shape.o: shape.h shape.cpp
	$(CC) $(CFLAGS) shape.cpp

tests.o: tests.cpp
	$(CC) $(CFLAGS) tests.cpp

main.o: point.o vector.o main.cpp
	$(CC) $(CFLAGS) main.cpp

check:
	make
	./main test

clean:
	\rm *.o 