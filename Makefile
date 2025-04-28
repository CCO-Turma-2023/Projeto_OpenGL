CC = g++

GLLIBS = -lopengl32 -lglew32 -lfreeglut -lglu32

all: geometry.cpp
	$(CC) geometry.cpp ./lib/utils.cpp ./objects/* -o geometry $(GLLIBS)

clean:
	rm -f geometry
