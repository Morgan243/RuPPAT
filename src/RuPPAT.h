#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"
#include "Descriptors.h"
#include "Render.h"
#include "Player.h"
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
		//CONSTRUCTOR(S)
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

		void addPlayer(Player* new_player);

		int addEntity(Entity_desc new_ent);

		int addObject(string spritePath, int x, int y, 
				int mass, float rotationRate,
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

		void firePlayersWeapon(int p_ID);

		//test the x and y Coordinates, invert velocities if desired and out of bounds
		bool testBounds(Entity_desc &testMe, bool invert);

	private:

		int WIDTH, HEIGHT, 
		    game_width, game_height,
		    gravitationalConstant,
		    baseID = 0;

		Uint32 thisTime;

		//if this goes true, all threads stop, engine stops
		bool done;
		
		Render* mainRender;

				
		//mutexes/lock used throughout
		pthread_mutex_t  pix_list_lock_2;
		pthread_rwlock_t  pix_rw_lock, 
				  mass_rw_lock,
				  ent_rw_lock,
				  player_rw_lock,
				  object_rw_lock;


		int centerX, centerY=0, xOrigin, yOrigin;

		//independent pixels on screen
		vector<Pixel_desc> pixelList;
		
		//points of mass on screen
		vector<Pixel_desc> pixelList_m;

		//indpenedt sprites on scree
		vector<Entity_desc> entList;

		//sprites that attract others 
		vector<Entity_desc> entList_m;

		//layers of the background
		vector<SDL_Surface *>backgroundLayers;

		//independent sprite that controlled by AI or human
		vector<Player *> players;

		vector<Object *> objectList;


		queue<Pixel_desc> toRender;

};
#endif
