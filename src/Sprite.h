// 				[Class] Sprite
//--------------------------------------------------------------------
// 		Handles all the sprite generation and manipulation.
//--------------------------------------------------------------------

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL2_rotozoom.h"
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

		explicit Sprite(string path_to_sprite,
				int numRotations, int startingAngle,
                SDL_Renderer *render_context);

		//copy constructor
		Sprite(const Sprite& src);
		Sprite(const Sprite* src);

		//assignment operator
		Sprite & operator=(const Sprite &src);

		~Sprite();

		void copyAll(SDL_Surface *dest, Sprite &sprDest) const;

		void copyBaseSprite(SDL_Surface *dest) const;

		void get_RotR_DegIncr_Angl(float &rotateRate,
			   						float &degreeInc,
								   	float &currAngle) const;
		
		void set_RotR_DegIncr_Angl(const float rotateRate,
			   						const float degreeInc,
								   	const float currAngle);

		void setBaseSprite();

		void setBaseSprite(const string path_to_sprite);

		void setBaseSprite(SDL_Surface *src);

		void generateRotations();

		void generateRotations(SDL_Renderer* use_renderer);

		void setRotationRate(const float rotRate);

		float getRotationRate();

		void incrementRotationRate();

		void decrementRotationRate();

		void updateSprite();

		float getAngle();
		
		int getAngleIndex();

		void setAngleIndex(const int angleIndex);

		void incrementAngleIndex();

		void decrementAngleIndex();

		SDL_Surface * getBaseSprite() const;

		SDL_Surface * getSprite();

		SDL_Surface * getSprite(const int angle);

		SDL_Texture * getBaseSprite_text() const;

		SDL_Texture * getSprite_text();

		SDL_Texture * getSprite_text(const int angle);


		Uint32 getPixel(const int x, const int y);

		Uint32 getPixel(const int x, const int y, const int rotation_i);

		void putPixel(const int x,
			   			const int y,
					   	Uint32 color,
					   	const int rotation_i);

        int getLargestDimension();

		void getDimensions(int &w, int &h);

		void getDimensions(int &w, int &h, const int rotation);

		void getDimensionsBpp(int &w,
			   					int &h,
							   	int &bpp)const;

		float lastErr, 
			  rotationalErrorAccum;

		void generateSpriteOutlines();

		void generatePixelSprite(bool print);

		vector<Pixel_desc> getPixelSprite();

		vector<CoOrd> outlineSprite();

		void clearPixelFromAll(const int start_rot_i, const int x, const int y);

		void clearPixel(const int rotation, const int x, const int y);

        static SDL_Renderer* render_context;
        
        //matrix of sprite: true where not alpha (color!)
        vector< vector<bool> > sprite_silhouette;

	protected:
		float rotationRate,
				degreeIncrement,
				currentAngleIndex_f;

		int currentAngleIndex;
		
		string sprite_path;

		SDL_Surface *base_sprite;
		vector<SDL_Surface *> rotations;	
		vector<Texture_Container> rotations_text;	
		vector<Pixel_desc> pixel_sprite;
		vector<Renderables_Cont> pixel_sprites;
};
#endif
