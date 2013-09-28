// 			[Class] RuPPAT
//--------------------------------------------------------------------
//| 	The primary class that brings everything together by handling
//|objects, pixels, players and their manipulations/control.
//--------------------------------------------------------------------
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL2_rotozoom.h"
#include "SDL2/SDL_image.h"
#include "Descriptors.h"
#include "Render.h"
#include "Primitives.h"
#include "Player.h"
#include "TextDriver.h"
#include <vector>
#include <queue>
#include <iostream>
#include <mutex>
#include <thread>
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

        RuPPAT(struct RuPPAT_Options options);

		//DECONSTRUCTOR
		~RuPPAT();

        //----Initialization and construction methods----
        void initLocks();

		int addObject(const string spritePath,
			   		  const int x,
					  const int y, 
				      const int mass,
					  const float rotationRate,
					  const float xVel,
					  const float yVel,
					  const string HB_path);

		void addPlayer(Player* new_player);

		int addPlayer(const string spritePath,
			   		  const int numRotations,
					  const int startingAngle, 
				      const float maxAccel,
					  const int x,
					  const int y,
					  const string HC_path);


        //----Core backend routines----
		void RK4(const float t,
                 const float dt);

		void RunRuPPAT();

		void parseSelectPixToSurface();

		void parsePlayersToSurface();

		void parseObjectsToSurface();


        //---player control and manipulations
		void accelPlayer(const int p_ID, const bool isForward);
		
		void turnPlayer(const int p_ID,
                        const bool isLeft,
                        const int numTurns);

		void turnPlayerToCoord(const int p_ID,
			   				   const int x,
							   const int y,
							   const int rate);

		void firePlayersWeapon(const int p_ID);

		//----Pixel creation and handling methods----
		void createPixElement(Pixel_desc *pixel); 

		void createPixElement(const int x,
							  const int y,	
							  const int color,
							  const int xAccel,
							  const int yAccel,
							  const int accLength,
							  const int dimFactor,
                              const int mass  );

		void setUpdateOnSelectPix(const int set);

		void handleAllDeleteMe();

		void handleDelete(const int index);

		Primitives *primitive_maker;
        
		//if this goes true, all threads stop, engine stops
		bool done;

        int calcTickInterval(int engineRate);
        int adjustRate(int delta);
        int adjust_dt(float delta);
		
	private:

        void (RuPPAT::*gfxPlacer)(void) = NULL;
        void wrap_gfxPlacer(void);
        void bounce_gfxPlacer(void);

        std::thread *rk4_th;
        int  screen_centX, screen_centY;

		int WIDTH, HEIGHT, 
		    game_width,
		   	game_height,
		    gravitationalConstant,
		    baseID = 0,
 			centerX,
		   	centerY=0,
		   	xOrigin,
		   	yOrigin;

        int engine_rate,
            interval,
            nextTick;

        float t = 0.0, dt = 0.004;

		Uint32 thisTime;

		
		Render* mainRender;
				
		//mutexes/lock used throughout
		pthread_mutex_t  pix_list_lock_2;
		pthread_rwlock_t  pix_rw_lock, 
						  mass_rw_lock,
						  ent_rw_lock,
						  player_rw_lock,
						  object_rw_lock;

        std::mutex render_job_mutex;

		//independent pixels on screen
		vector<Pixel_desc> pixelList;
		
		//points of mass on screen
		vector<Pixel_desc> pixelList_m;

		//indpenedt sprites on scree
		vector<Entity_desc> entList;

		//sprites that attract others 
		vector<Entity_desc> entList_m;

		//layers of the background
		vector<SDL_Texture *>backgroundLayers;

		//independent sprite that controlled by AI or human
		vector<Player *> players;

		vector<Object *> objectList;

		queue<Pixel_desc> toRender;

        TextDriver *text_driver;

        std::queue< RenderJob_Container* > render_jobs;
};
#endif
