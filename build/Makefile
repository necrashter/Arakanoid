LDLIBS=-lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer
CXXFLAGS=-I../include -g

all: main

main: ../src/main.cpp ../src/load_resources.o ../src/sprite.o ../src/entity.o ../src/game_entities.o
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -f main
	rm -f ../src/*.o
