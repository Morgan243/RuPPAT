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

		//copy constructor
		Sprite(const Sprite& src);

		//assignment operator
		Sprite & operator=(const Sprite &src);

		~Sprite();

		void copyAll(SDL_Surface *dest, Sprite &sprDest) const;
		void copyBaseSprite(SDL_Surface *dest) const;

		void get_RotR_DegIncr_Angl(float &rotateRate, float &degreeInc
						, float &currAngle) const;
		
		void set_RotR_DegIncr_Angl(float rotateRate, float degreeInc
						, float currAngle);
		//use prtoected sprite path member
		void setBaseSprite();

		//load sprite from path
		void setBaseSprite(string path_to_sprite);

		//copy from surface
		void setBaseSprite(SDL_Surface *src);

		//make all rotations based on protected members
		void generateRotations();

		void setRotationRate(float rotRate);

		float getRotationRate();

		void incrementRotationRate();

		void decrementRotationRate();

		void updateSprite();

		float getAngle();

		void setAngleIndex(int angleIndex);

		void incrementAngleIndex();

		void decrementAngleIndex();

		SDL_Surface * getBaseSprite() const;


		SDL_Surface * getSprite();

		SDL_Surface * getSprite(int angle);

		Uint32 getPixel(int x, int y);

		Uint32 getPixel(int x, int y, int rotation_i);

		void putPixel(int x, int y, Uint32 color, int rotation_i);

		void getDimensions(int &w, int &h);
		void getDimensionsBpp(int &w, int &h, int &bpp)const;

		float lastErr;
		float rotationalErrorAccum;

		void generateSpriteOutlines();
		vector<CoOrd> outlineSprite();

		void clearPixelFromAll(int start_rot_i, int x, int y);
		void clearPixel(int rotation, int x, int y);



	protected:
		float rotationRate;
		float degreeIncrement;
	
		int currentAngleIndex;
		float currentAngleIndex_f;
		

		SDL_Surface *base_sprite;
		vector<SDL_Surface *> rotations;	
		
		//unsigned int ** pixMaps;

		string sprite_path;
};
#endif
