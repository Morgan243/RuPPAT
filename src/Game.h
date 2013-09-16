// 			[Class]	Game
//--------------------------------------------------------------------
//| 	Basic logic for gameplay should exist here. Things like handling
//|player input and initializing game specific events.
//--------------------------------------------------------------------
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL2_rotozoom.h"
#include "SDL2/SDL_image.h"
#include "Descriptors.h"
#include "Render.h"
#include "RuPPAT.h"
#include "Player.h"
//#include "ObjectFactory.h"
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

		Game(vector<section> configSection);	

		//DECONSTRUCTOR
		~Game();

        void initGame(vector<section> configSections);

        void initGame_lua(vector<section> configSections);

        void initParameters();

		void initEvent(Event_desc &initMe);

		void createPixElement(int x, int y,	
								int color,
								int xAccel,
								int yAccel,
								int accLength,
								int dimFactor, int mass  );

		void parseSelectPixToSurface();

		void updateSelectPix();

		void setUpdateOnSelectPix(int set);
			
		void run(string selection, string option);	

        void game_loop();

		void handleEvents(Event_desc &mainEvents);

		void rotateSpriteToCoor(int p_ID,
			   					int x,
							   	int y,
							   	int turnRate);

	private:
		SDL_Event event;	
        
        //declare and init SDL events structure
        Event_desc mainEvents;

		bool done,
			 space;

		RuPPAT *engine;	

		vector<Player *> players;

		int x, y, 
			up_count, down_count, left_count, right_count,
			key_count_limit, defaultTurnAmnt, fastTurn,
			w_count, s_count, a_count, d_count, space_count,
			mouse_x, mouse_y;

		bool  k_UP, k_DOWN, k_LEFT, k_RIGHT,
				k_lCTRL, k_lMOUSE;
};
#endif

