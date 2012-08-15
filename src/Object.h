//Non player objects and such
//
//This is a parent class

#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"
#include "math.h"

#include "Descriptors.h"

#include <vector>
#include <iostream>

#ifndef OBJECT_H
#define OBJECT_H

using namespace std;

class Object
{
	public:

	explicit Object(string sprite_path, int start_x, int start_y, int start_mass);
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
#endif
