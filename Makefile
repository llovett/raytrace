FLAGS = -I/usr/include/GL -g
LIBS = -L/usr/include -lglut -lglui -lGLU -lGL -lm

APPS = trace

all: $(APPS)

trace: trace.cpp trace.h
	g++ trace.cpp -o $(APPS) $(FLAGS) $(LIBS)

clean:
	@rm -rf $(APPS)

