CC = g++

GLLIBS = -lglut -lGLEW -lGL

all: geometry.cpp
	$(CC) geometry.cpp ./lib/utils.cpp ./objects/* -o geometry $(GLLIBS)

clean:
	rm -f geometry
