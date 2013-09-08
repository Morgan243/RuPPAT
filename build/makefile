CXX = g++
# Update these paths to match your installation
# You may also need to update the linker option rpath, which sets where to look for
# the SDL2 libraries at runtime to match your install
SDL_LIB = -lpthread -lm -lX11 -ggdb -llua -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_gfx -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/local/include
CXXFLAGS = -c -std=c++11 $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)
EXE = test

SRC_DIR = ../src/

all: $(EXE)

$(EXE): main.o Common.o $(SRC_DIR)Descriptors.h Render.o LuaParser.o Game.o \
	RuPPAT.o Primitives.o Player.o Object.o Sprite.o Missile.o TextDriver.o \
	PhysFuncs.o
	$(CXX) $^ $(LDFLAGS) -o $@

main.o: $(SRC_DIR)main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

Game.o: $(SRC_DIR)Game.cpp $(SRC_DIR)Game.h
	$(CXX) $(CXXFLAGS) $< -o $@

RuPPAT.o: $(SRC_DIR)RuPPAT.cpp $(SRC_DIR)RuPPAT.h
	$(CXX) $(CXXFLAGS) $< -o $@

Primitives.o: $(SRC_DIR)Primitives.cpp $(SRC_DIR)Primitives.h
	$(CXX) $(CXXFLAGS) $< -o $@

Player.o: $(SRC_DIR)Player.cpp $(SRC_DIR)Player.h
	$(CXX) $(CXXFLAGS) $< -o $@

Object.o: $(SRC_DIR)Object.cpp $(SRC_DIR)Object.h
	$(CXX) $(CXXFLAGS) $< -o $@

Sprite.o: $(SRC_DIR)Sprite.cpp $(SRC_DIR)Sprite.h
	$(CXX) $(CXXFLAGS) $< -o $@

Missile.o: $(SRC_DIR)Missile.cpp $(SRC_DIR)Missile.h
	$(CXX) $(CXXFLAGS) $< -o $@

TextDriver.o: $(SRC_DIR)TextDriver.cpp $(SRC_DIR)TextDriver.h
	$(CXX) $(CXXFLAGS) $< -o $@

PhysFuncs.o: $(SRC_DIR)PhysFuncs.cpp $(SRC_DIR)PhysFuncs.h
	$(CXX) $(CXXFLAGS) $< -o $@

Common.o: $(SRC_DIR)Common.cpp $(SRC_DIR)Common.h
	$(CXX) $(CXXFLAGS) $< -o $@
	
Render.o: $(SRC_DIR)Render.cpp $(SRC_DIR)Render.h
	$(CXX) $(CXXFLAGS) $< -o $@

LuaParser.o: $(SRC_DIR)LuaParser.cpp $(SRC_DIR)LuaParser.h
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)
