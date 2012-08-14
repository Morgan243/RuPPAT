####################################################################################
#This makefile builds the RuPPAT engine and a series of small interactive test games
#
#Author: Morgan Stuart
#
##################################################################################


CFLAGS=-lm -lSDL -lSDL_image -lSDL_gfx -lm -fopenmp -lpthread -lX11 -ggdb
COMP=g++
BUILD_DIR=../build/
BUILD_OUT=-o $(BUILD_DIR)
all: test

test: main.o Render.o Game.o RuPPAT.o PhysFuncs.o Descriptors.o Player.o Object.o  
	$(COMP) main.o Render.o Game.o RuPPAT.o PhysFuncs.h Descriptors.h Player.o Object.o $(CFLAGS) $(BUILD_OUT)test && mv *.o *.gch $(BUILD_DIR) && ln -s $(BUILD_DIR)test ./test

main.o: main.cpp
	$(COMP) -c main.cpp $(CFLAGS)

Render.o: Render.h Render.cpp
	$(COMP) -c Render.h Render.cpp $(CFLAGS) 

Game.o: Game.h Game.cpp
	$(COMP) -c Game.h Game.cpp $(CFLAGS)

RuPPAT.o: RuPPAT.cpp RuPPAT.h
	$(COMP) -c RuPPAT.h RuPPAT.cpp $(CFLAGS)

PhysFuncs.o: PhysFuncs.h
	$(COMP) -c PhysFuncs.h $(CFLAGS)

Descriptors.o: Descriptors.h
	$(COMP) -c Descriptors.h $(CFLAGS)

Player.o: Player.h Player.cpp
	$(COMP) -c Player.h Player.cpp $(CFLAGS)

Object.o: Object.h Object.cpp
	$(COMP) -c Object.h Object.cpp $(CFLAGS)

clean: 
	rm $(BUILD_DIR)* test 


#test : main.cpp Render.h Render.cpp Game.h Game.cpp RuPPAT.cpp RuPPAT.h  PhysFuncs.h Descriptors.h Player.cpp Player.h Object.h Object.cpp
#	g++ main.cpp Render.h Render.cpp Game.h Game.cpp RuPPAT.h RuPPAT.cpp  PhysFuncs.h Descriptors.h Player.h Player.cpp Object.h Object.cpp -o test -lm -lSDL -lSDL_image -lSDL_gfx -lm -fopenmp -lpthread -lX11 -ggdb
