//Non player objects and stuff
//

#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"
#include "math.h"

#include "Descriptors.h"

#include <vector>
#include <iostream>

using namespace std;

class Object
{
	public:
		Object(string sprite_path, int start_x, int start_y, int start_mass);
		~Object();


		Mass_desc getDesc();

		SDL_Surface *getSprite();


		int getX();
		int getY();

	private:

		float x, y;
		float xVel, yVel;//velocity vector
		float xAcc, yAcc;//acceleration vector
		int xGacc,yGacc; //grav acceleration
	
		int mass; //mass	

		Mass_desc descriptor;
		SDL_Surface *sprite;	
};
