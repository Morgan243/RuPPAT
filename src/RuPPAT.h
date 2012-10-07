#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"
#include "Descriptors.h"
#include "Render.h"
#include <vector>
#include <queue>
#include <iostream>
#ifndef RuPPAT_H
#define RuPPAT_H


#define BPP 32


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

		RuPPAT(int width, int height, int bpp, unsigned int flags,
			 vector<string> bkg_paths);

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
			float maxAccel, int x, int y, string HC_path);

	int addEntity(Entity_desc new_ent);

	int addObject(string spritePath, int x, int y, int mass, float rotationRate,
				float xVel, float yVel, string HB_path);

	Entity_desc RK4_entity(Entity_desc ent);

	void RK4_all(float t, float dt);

	void runDemos(void *selection);

	void RK4(float t, float dt);

	void RK4_parse();

	void runPPAT(bool *mainDone, Event_desc *mainEvents, 
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

	//	Uint32 thisTime;
	//	Uint32 lastTime;
	//	Uint32 deltaTime;

};
#endif
