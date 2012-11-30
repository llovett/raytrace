FLAGS = -I/usr/include/GL -g
LIBS = -L/usr/include -lglut -lglui -lGLU -lGL -lm
CPP=g++
APPS = trace

all: $(APPS)

shape: Shape.h Shape.cpp
	$(CPP) -c Shape.cpp -o Shape.o $(FLAGS) $(LIBS)

sphere: Sphere.h Sphere.cpp shape
	$(CPP) -c Sphere.cpp -o Sphere.o $(FLAGS) $(LIBS)

box: box.h box.cpp shape
	$(CPP) -c box.cpp -o Box.o $(FLAGS) $(LIBS)

trace: trace.cpp trace.h box sphere
	$(CPP) trace.cpp Box.o Sphere.o Shape.o -o $(APPS) $(FLAGS) $(LIBS)

test: Sphere.h Sphere.cpp shape
	$(CPP) Sphere.cpp Shape.o $(FLAGS) $(LIBS)
	./a.out
	@rm -f a.out

clean:
	@rm -rf $(APPS)
	@rm -rf *.o
