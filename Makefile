LDLIBS=-lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer
CXXFLAGS=-I./include

all: main

main: src/main.cpp src/load_resources.o
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o ./build/$@
