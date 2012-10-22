####################################################################################
#This makefile builds the RuPPAT engine and a series of small interactive test games
#
#Author: Morgan Stuart
#
##################################################################################


CFLAGS=-lm -lSDL -lSDL_image -lSDL_gfx -lm -fopenmp -lpthread -lX11 -std=c++11 -ggdb
COMP=g++
BUILD_DIR=./
BUILD_OUT=-o $(BUILD_DIR)
SOURCE_DIR=../src/
all: test

test: main.o Render.o Game.o RuPPAT.o PhysFuncs.o Descriptors.o Player.o Object.o Sprite.o Missile.o Primitives.o
	$(COMP) $(BUILD_DIR)main.o $(BUILD_DIR)Render.o $(BUILD_DIR)Game.o $(BUILD_DIR)RuPPAT.o $(SOURCE_DIR)PhysFuncs.h \
	$(SOURCE_DIR)Descriptors.h $(BUILD_DIR)Player.o $(BUILD_DIR)Object.o $(BUILD_DIR)Sprite.o $(BUILD_DIR)Missile.o \
	$(BUILD_DIR)Primitives.o $(CFLAGS) $(BUILD_OUT)test  

main.o: $(SOURCE_DIR)main.cpp
	$(COMP) -c $(SOURCE_DIR)main.cpp $(CFLAGS) 

Render.o: $(SOURCE_DIR)Render.h $(SOURCE_DIR)Render.cpp
	$(COMP) -c $(SOURCE_DIR)Render.h $(SOURCE_DIR)Render.cpp $(CFLAGS) 

Game.o: $(SOURCE_DIR)Game.h $(SOURCE_DIR)Game.cpp
	$(COMP) -c $(SOURCE_DIR)Game.h $(SOURCE_DIR)Game.cpp $(CFLAGS) 

RuPPAT.o: $(SOURCE_DIR)RuPPAT.cpp $(SOURCE_DIR)RuPPAT.h
	$(COMP) -c $(SOURCE_DIR)RuPPAT.h $(SOURCE_DIR)RuPPAT.cpp $(CFLAGS)

PhysFuncs.o: $(SOURCE_DIR)PhysFuncs.h
	$(COMP) -c $(SOURCE_DIR)PhysFuncs.h $(CFLAGS) 

Descriptors.o: $(SOURCE_DIR)Descriptors.h
	$(COMP) -c $(SOURCE_DIR)Descriptors.h $(CFLAGS) 

Player.o: $(SOURCE_DIR)Player.h $(SOURCE_DIR)Player.cpp
	$(COMP) -c $(SOURCE_DIR)Player.h $(SOURCE_DIR)Player.cpp $(CFLAGS) 

Object.o: $(SOURCE_DIR)Object.h $(SOURCE_DIR)Object.cpp
	$(COMP) -c $(SOURCE_DIR)Object.h $(SOURCE_DIR)Object.cpp $(CFLAGS) 

Sprite.o: $(SOURCE_DIR)Sprite.h $(SOURCE_DIR)Sprite.cpp
	$(COMP) -c $(SOURCE_DIR)Sprite.h $(SOURCE_DIR)Sprite.cpp $(CFLAGS)

Missile.o: $(SOURCE_DIR)Missile.h $(SOURCE_DIR)Missile.cpp
	$(COMP) -c $(SOURCE_DIR)Missile.h $(SOURCE_DIR)Missile.cpp $(CFLAGS)

Primitives.o: $(SOURCE_DIR)Primitives.h $(SOURCE_DIR)Primitives.cpp
	$(COMP) -c $(SOURCE_DIR)Primitives.h $(SOURCE_DIR)Primitives.cpp $(CFLAGS)

clean: 
	rm $(BUILD_DIR)*.o $(SOURCE_DIR)*.gch $(BUILD_DIR)test

