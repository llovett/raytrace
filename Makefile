FLAGS = -I/usr/include/GL -g
LIBS = -L/usr/include -lglut -lglui -lGLU -lGL -lm
CPP=g++
APPS = trace

all: $(APPS)

matrix: Matrix.h Matrix.cpp
	$(CPP) -c Matrix.cpp -o Matrix.o $(FLAGS) $(LIBS)

shape: Shape.h Shape.cpp
	$(CPP) -c Shape.cpp -o Shape.o $(FLAGS) $(LIBS)

sphere: Sphere.h Sphere.cpp shape
	$(CPP) -c Sphere.cpp -o Sphere.o $(FLAGS) $(LIBS)

plane: Plane.h Plane.cpp shape matrix
	$(CPP) -c Plane.cpp -o Plane.o $(FLAGS) $(LIBS)

trace: trace.cpp trace.h sphere plane
	$(CPP) trace.cpp Sphere.o Shape.o Matrix.o Plane.o -o $(APPS) $(FLAGS) $(LIBS)

test: Sphere.h Sphere.cpp shape
	$(CPP) Sphere.cpp Shape.o $(FLAGS) $(LIBS)
	./a.out
	@rm -f a.out

clean:
	@rm -rf $(APPS)
	@rm -rf *.o
