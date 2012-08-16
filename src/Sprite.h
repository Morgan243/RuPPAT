//Class used to handle all the sprite generation and manipulation

#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"

#include <iostream>
#include <vector>
#ifndef SPRITE_H
#define SPRITE_H

using namespace std;

class Sprite
{
	public:
		explicit Sprite(string path_to_sprite);
		explicit Sprite(string path_to_sprite,
				int numRotations, int startingAngle);
		~Sprite();

		void setRotationRate(int rotRate);

		float getRotationRate();

		void incrementRotationRate();

		void decrementRotationRate();

		void updateSprite();

		float getAngle();

		void setAngleIndex(int angleIndex);

		void incrementAngleIndex();

		void decrementAngleIndex();

		SDL_Surface * getSprite();

		SDL_Surface * getSprite(int angle);

		float lastErr;
		float rotationalErrorAccum;

	protected:
		float rotationRate;
		float degreeIncrement;
	
		int currentAngleIndex;
		float currentAngleIndex_f;
		

		SDL_Surface *sprite_surf;
		vector<SDL_Surface *> rotations;	
		
};
#endif
