//Object is the base class for moveable Entities
// 	an object doesnt necessarily have a sprite.
// 	you must derive from object and compose a 
// 	class with the Sprite class in it
//
// 	Thus, Object can be used for invisible masses
// 	and other effects
//
//This is a parent class

#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"
#include "math.h"

#include "Descriptors.h"
#include "Sprite.h"

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

		//for compatability, gets a mass desc out of Entity_desc
		Mass_desc getDesc();
		Entity_desc getDescriptor();

		SDL_Surface *getSprite();


		int getX();
		int getY();

		float getXvel();
		float getYvel();

		float getXacc();
		float getYacc();

		void accelForward();	
		void accelBackward();

	protected:

		Entity_desc descriptor;
		Sprite sprite_;
		SDL_Surface *sprite;	
};
#endif
