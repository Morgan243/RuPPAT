//Class used to handle all the sprite generation and manipulation

#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"
#include "Descriptors.h"

#include <iostream>
#include <vector>
#ifndef SPRITE_H
#define SPRITE_H

using namespace std;

class Sprite
{
	public:
		explicit Sprite();
		explicit Sprite(string path_to_sprite);
		explicit Sprite(string path_to_sprite,
				int numRotations, int startingAngle);
		~Sprite();

		void setRotationRate(float rotRate);

		float getRotationRate();

		void incrementRotationRate();

		void decrementRotationRate();

		void updateSprite();

		float getAngle();

		void setAngleIndex(int angleIndex);

		void incrementAngleIndex();

		void decrementAngleIndex();

		SDL_Surface * getBaseSprite();

		SDL_Surface * getSprite();

		SDL_Surface * getSprite(int angle);

		Uint32 getPixel(int x, int y);

		void putPixel(int x, int y, Uint32 color, int screenID);

		void getDimensions(int &w, int &h);

		float lastErr;
		float rotationalErrorAccum;

		void generateSpriteOutlines();
		vector<CoOrd> outlineSprite();

	protected:
		float rotationRate;
		float degreeIncrement;
	
		int currentAngleIndex;
		float currentAngleIndex_f;
		

		SDL_Surface *sprite_surf;
		vector<SDL_Surface *> rotations;	
		
		unsigned int ** pixMaps;
};
#endif
