LDLIBS=-lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer
CXXFLAGS=-I./include

all: main

main: src/main.cpp src/load_resources.o src/sprite.o
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o ./build/$@

clean:
	rm -f build/main
	rm -f src/*.o
