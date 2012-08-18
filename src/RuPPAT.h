#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"
#include "Descriptors.h"
#include "Render.h"
//#include "Player.h"
//#include "PhysFuncs.h"
#include <vector>
#include <queue>
#include <iostream>
#ifndef RuPPAT_H
#define RuPPAT_H


//#define WIDTH 1024
//#define HEIGHT 768
#define BPP 32




	void *testThread(void *args);

using namespace std;

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//________________________________________________________________
//CLASS::::RuPPAT
class RuPPAT
{
	public:
		//CONSTRUCTOR
		RuPPAT(int width,int height,int bpp, unsigned int flags,
			 string bkg_paths[], int num_bkgs);
		//DECONSTRUCTOR
		~RuPPAT();

	//push pixel to pixel_list_m
	void createPixElement(Pixel_desc *pixel); 

	void parseSelectPixToSurface();

	void parsePlayersToSurface();

	void parseObjectsToSurface();

	void setUpdateOnSelectPix(int set);

	void handleAllDeleteMe();

	void handleDelete(int index);

	int applyDimming(Pixel_desc &pix_t);

	int addPlayer(string spritePath, int numRotations, int startingAngle, 
			float maxAccel, int x, int y);

	int addEntity(Entity_desc new_ent);

int addObject(string spritePath, int x, int y, int mass, float rotationRate,
		float xVel, float yVel);

	Entity_desc RK4_entity(Entity_desc ent);

	void RK4_all(float t, float dt);

	void runDemos(void *selection);

	void RK4_parse(SDL_Surface* background);

	void runPPAT(bool *mainDone, Event_desc &mainEvents, 
			string selection, string option);

	void createPixElement(  int x, int y,	
				int color,
				int xAccel,
				int yAccel,
				int accLength,
				int dimFactor, int mass  );

	void updateSelectPix();

	void putPixel(int x, int y, unsigned int color, int id);

	void putSprite(int x, int y, SDL_Surface *sprite);

	void accelPlayer(int p_ID, bool isForward);
	
	void turnPlayer(int p_ID, bool isLeft, int numTurns);

	void turnPlayerToCoord(int p_ID, int x, int y, int rate);



	private:

		Uint32 thisTime;
		Uint32 lastTime;
		Uint32 deltaTime;

	
	

	bool orbGrav;//is orbital gravity on?
	int gravitationalConstant;
	int IDbase;//value of next ID 

};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-v-v-v-v-v-___HELPER STRUCTURES AND FUNCTIONS___-v-v-v-v-v-v-

	//.....[HELPER]createPixElement(Pixel_desc)
	struct cPE_helper
		{
			void *context;
			Pixel_desc *pixel;	
		};

	static void *createPixElement_helper(void *cpe_helper)
		{
		cPE_helper *helper = (cPE_helper*) cpe_helper;
		Pixel_desc *pix = helper->pixel;
		((RuPPAT *)helper->context)->createPixElement(pix);
		return NULL;
		}




	//.....[HELPER]updateSelectPix	
//	static void *updateSelectPix_helper(void *context)
//		{
//		((RuPPAT*)context)->updateSelectPix();
//		return NULL;
//		}



	//.....[HELPER]runDemos(char selection)
	struct helper_char
		{
			void *context;
			char *character;
		};

	static void *runDemos_helper(void *helper_c)
		{
		helper_char *helper = (helper_char*) helper_c;
		char *c = helper->character;
		((RuPPAT *)helper->context)->runDemos(c);
		return NULL;
		}


	
	//.....[HELPER]RK4_pars
	struct RK4_parse_helper_arg
		{
			void *context;
			//vector<SDL_Surface *> bkg_layers;
			SDL_Surface* backg;
		};

	static void *RK4_parse_helper(void *helper_in)
		{
		RK4_parse_helper_arg* helper = (RK4_parse_helper_arg*) helper_in;
		
		((RuPPAT*)helper->context)->RK4_parse(helper->backg);
		return NULL;
		};



	//.....[HELPER]runPPAT
	struct runPPAT_helperArgs
		{
			void *context;
			bool *mDone;
			Event_desc *mEvents;
			string selection;
			string option;
		};

	static void *runPPAT_helper(void *helper_args)
		{
		runPPAT_helperArgs *helper = (runPPAT_helperArgs*) helper_args;
		bool *d = helper->mDone;
		Event_desc *e = helper->mEvents;
		((RuPPAT *)helper->context)->runPPAT(d, *e,
				helper->selection, helper->option);
		return NULL;
		}


#endif
