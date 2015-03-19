OBJS = point.o normal.o localgeo.o color.o vector.o ray.o BRDF.o light.o viewplane.o camera.o matrix.o transformation.o ellipsoid.o triangle.o tests.o main.o 

# Basics Compiling Stuff
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

CXXFLAGS = -I lib/boost_1_57_0/
LDOPTS = -L./ -lfreeimage


all: $(OBJS) 
	$(CC) $(CXXFLAGS) $(LFLAGS) $(LDOPTS) $(OBJS) -o raytracer

point.o: point.h point.cpp
	$(CC) $(CFLAGS) point.cpp

normal.o: normal.h normal.cpp
	$(CC) $(CFLAGS) normal.cpp

localgeo.o: localgeo.h localgeo.cpp
	$(CC) $(CFLAGS) localgeo.cpp

color.o: color.h color.cpp
	$(CC) $(CFLAGS) color.cpp

vector.o: vector.h vector.cpp
	$(CC) $(CFLAGS) vector.cpp

ray.o: ray.h ray.cpp
	$(CC) $(CFLAGS) ray.cpp

BRDF.o: BRDF.h BRDF.cpp
	$(CC) $(CFLAGS) BRDF.cpp

light.o: light.h light.cpp
	$(CC) $(CFLAGS) light.cpp

viewplane.o: viewplane.h viewplane.cpp
	$(CC) $(CFLAGS) viewplane.cpp

camera.o: camera.h camera.cpp
	$(CC) $(CFLAGS) camera.cpp

matrix.o: matrix.h matrix.cpp
	$(CC) $(CFLAGS) matrix.cpp

transformation.o: transformation.h transformation.cpp
	$(CC) $(CFLAGS) transformation.cpp

ellipsoid.o: ellipsoid.h ellipsoid.cpp shape.h
	$(CC) $(CFLAGS) ellipsoid.cpp

triangle.o: triangle.h triangle.cpp shape.h
	$(CC) $(CFLAGS) triangle.cpp

tests.o: tests.h tests.cpp
	$(CC) $(CFLAGS) tests.cpp

main.o: point.o vector.o main.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS) main.cpp

check:
	make
	./raytracer test

clean:
	\rm *.o raytracer