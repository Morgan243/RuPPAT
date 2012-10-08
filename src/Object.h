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
				 float xVel, float yVel, string HC_path);

//explicit Object(string sprite_path, int start_x, int start_y, int start_mass,
//			int num_rotations, int starting_angle,
//				 float xVel, float yVel, string HB_path);
		~Object();

		Entity_desc getDescriptor();
		void setDescriptor(Entity_desc new_desc);
			
		SDL_Surface *getSprite();
		SDL_Surface *getSprite(int angle);


		void setID(int id);
		int getX();
		int getY();

		void setXY(int x, int y);

		float getXvel();
		float getYvel();

		float getXacc();
		float getYacc();

		void accelForward();	
		void accelBackward();

		void setAccelVectors(bool forward);

		void getXY_exhaust(float &xVel, float &yVel);

	

		void setRotation_rate(float rotRate);

		float getRotation_rate();

		void incrementRotation_rate();

		void decrementRotation_rate();

		void updateSprite();

		float getAngle();

		void setAngle_index(int angleIndex);

		void incrementAngle_index();

		void decrementAngle_index();

		void buildHitBoxes_fromSprite();

		void buildHitBoxes_fromLayer(SDL_Surface* hitBoxLayout);

		bool checkHits(float &xVel, float &yVel, Entity_desc &check_ent);


		Sprite sprite, hitCircleSprite;

		float lastErr;
		float rotationalErrorAccum;
		
	protected:
		float exhaustX, exhaustY;
		Entity_desc descriptor;
};
#endif
