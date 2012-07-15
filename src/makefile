
CFLAGS=-lm -lSDL -lSDL_image -lSDL_gfx -lm -fopenmp -lpthread -lX11 -ggdb

all: test

test: main.o Render.o Game.o RuPPAT.o PhysFuncs.o Descriptors.o Player.o Object.o  
	g++ main.o Render.o Game.o RuPPAT.o PhysFuncs.h Descriptors.h Player.o Object.o $(CFLAGS) -o test

main.o: main.cpp
	g++ -c main.cpp $(CFLAGS)

Render.o: Render.h Render.cpp
	g++ -c Render.h Render.cpp $(CFLAGS)

Game.o: Game.h Game.cpp
	g++ -c Game.h Game.cpp $(CFLAGS)

RuPPAT.o: RuPPAT.cpp RuPPAT.h
	g++ -c RuPPAT.h RuPPAT.cpp $(CFLAGS)

PhysFuncs.o: PhysFuncs.h
	g++ -c PhysFuncs.h $(CFLAGS)

Descriptors.o: Descriptors.h
	g++ -c Descriptors.h $(CFLAGS)

Player.o: Player.h Player.cpp
	g++ -c Player.h Player.cpp $(CFLAGS)

Object.o: Object.h Object.cpp
	g++ -c Object.h Object.cpp $(CFLAGS)

clean: 
	rm *.o test


#test : main.cpp Render.h Render.cpp Game.h Game.cpp RuPPAT.cpp RuPPAT.h  PhysFuncs.h Descriptors.h Player.cpp Player.h Object.h Object.cpp
#	g++ main.cpp Render.h Render.cpp Game.h Game.cpp RuPPAT.h RuPPAT.cpp  PhysFuncs.h Descriptors.h Player.h Player.cpp Object.h Object.cpp -o test -lm -lSDL -lSDL_image -lSDL_gfx -lm -fopenmp -lpthread -lX11 -ggdb
