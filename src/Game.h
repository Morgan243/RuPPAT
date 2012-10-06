#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"
#include "Descriptors.h"
#include "Render.h"
#include "RuPPAT.h"
#include "Player.h"
#include <vector>
#include <iostream>
#ifndef GAME_H
#define GAME_H

#define WIDTH 1024
#define HEIGHT 768
#define BPP 32
using namespace std;


//|||||||||||||||||||||||||||||||||||||||||||||||||
//CLASS:::GAME
class Game
{
public:

	//CONSTRUCTOR
	Game();
	Game(int WIDTH_cl, int HEIGHT_cl);

	Game(RunOptions options);	

	Game(vector<section> configSection);	


	//DECONSTRUCTOR
	~Game();

	void initEvent(Event_desc &initMe);


	void createPixElement(  int x, int y,	
				int color,
				int xAccel,
				int yAccel,
				int accLength,
				int dimFactor, int mass  );

	
	void parseSelectPixToSurface();


	void updateSelectPix();


	void setUpdateOnSelectPix(int set);

		
	void run(string selection, string option);	

	float runDemos(string selection, string option1, string option2,
		        int x1, int y1, int x2, int y2, int x3, int y3);

	void handleEvents(Event_desc &mainEvents);

	void rotateSpriteToCoor(int p_ID, int x, int y, int turnRate);

private:
	SDL_Event event;	

	bool done;
	bool space;//
	RuPPAT *engine;	

	vector<Player *> players;
};
#endif

