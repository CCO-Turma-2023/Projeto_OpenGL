CC = g++

UNAME_S := $(shell uname -s)

ifeq ($(OS),Windows_NT)
    GLLIBS = -lopengl32 -lglew32 -lfreeglut -lglu32
else ifeq ($(UNAME_S),Linux)
    GLLIBS = -lglut -lGLEW -lGL
else
    $(error Sistema operacional não suportado)
endif

all: geometry.cpp
	$(CC) geometry.cpp ./lib/utils.cpp ./objects/* -o geometry $(GLLIBS)

clean:
	rm -f geometry