//Object is the base class for moveable Entities
//
//
//This is a parent class but is useable for basics 

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

explicit Object(string sprite_path, int start_x, int start_y, int start_mass,
			int num_rotations, int starting_angle);

explicit Object(string sprite_path, int start_x, int start_y, int start_mass,
			int num_rotations, int starting_angle,
				 float xVel, float yVel);
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

		void setDescriptor(Entity_desc new_desc);

		Sprite sprite;

	protected:

		Entity_desc descriptor;
};
#endif
