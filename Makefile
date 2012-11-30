FLAGS = -I/usr/include/GL -g
LIBS = -L/usr/include -lglut -lglui -lGLU -lGL -lm
CPP=g++
APPS = trace

all: $(APPS)

box: box.h box.cpp
	$(CPP) -c box.cpp -o box.o $(FLAGS) $(LIBS)

trace: trace.cpp trace.h box
	$(CPP) trace.cpp box.o -o $(APPS) $(FLAGS) $(LIBS)

clean:
	@rm -rf $(APPS)

