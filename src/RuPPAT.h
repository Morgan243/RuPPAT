// 			[Class] RuPPAT
//--------------------------------------------------------------------
//| 	The primary class that brings everything together by handling
//|objects, pixels, players and their manipulations/control.
//--------------------------------------------------------------------
#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"
#include "Descriptors.h"
#include "Render.h"
#include "Primitives.h"
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
		RuPPAT(int width,
			   int height,
			   int bpp,
			   unsigned int flags,
			   string bkg_paths[],
			   int num_bkgs);

		RuPPAT(int width,
			   int height,
			   int bpp,
			   unsigned int flags,
			   vector<string> bkg_paths);

		//DECONSTRUCTOR
		~RuPPAT();

		//push pixel to pixel_list_m
		void createPixElement(Pixel_desc *pixel); 

		void parseSelectPixToSurface();

		void parsePlayersToSurface();

		void parseObjectsToSurface();

		void setUpdateOnSelectPix(const int set);

		void handleAllDeleteMe();

		void handleDelete(const int index);

		int applyDimming(Pixel_desc &pix_t);

		int addPlayer(const string spritePath,
			   		  const int numRotations,
					  const int startingAngle, 
				      const float maxAccel,
					  const int x,
					  const int y,
					  const string HC_path);

		void addPlayer(Player* new_player);

		int addEntity(Entity_desc new_ent);

		int addObject(const string spritePath,
			   		  const int x,
					  const int y, 
				      const int mass,
					  const float rotationRate,
					  const float xVel,
					  const float yVel,
					  const string HB_path);

		Entity_desc RK4_entity(Entity_desc ent);

		void RK4_all(const float t,
                     const float dt);

		void RK4(const float t,
                 const float dt);

		void RK4_parse();

		void runPPAT(const bool *mainDone,
			   		 const Event_desc *mainEvents, 
					 const string selection,
					 const string option);

		void createPixElement(const int x,
							  const int y,	
							  const int color,
							  const int xAccel,
							  const int yAccel,
							  const int accLength,
							  const int dimFactor,
                              const int mass  );

		void updateSelectPix();

		void putPixel(const int x,
			   		  const int y,
					  const unsigned int color,
					  const int id);

		void accelPlayer(const int p_ID, const bool isForward);
		
		void turnPlayer(const int p_ID,
                        const bool isLeft,
                        const int numTurns);

		void turnPlayerToCoord(const int p_ID,
			   				   const int x,
							   const int y,
							   const int rate);

		void firePlayersWeapon(const int p_ID);

		Primitives *primitive_maker;
		
	private:

		int WIDTH, HEIGHT, 
		    game_width,
		   	game_height,
		    gravitationalConstant,
		    baseID = 0,
 			centerX,
		   	centerY=0,
		   	xOrigin,
		   	yOrigin;

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
