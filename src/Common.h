// 				[Class] Common
//--------------------------------------------------------------------
//| 	A static class that contains methods and related members
//|for common operations that are used throughout the code base.
//|These are methods and members that are needed in a variety of places
//|at many different levels of abstraction.
//--------------------------------------------------------------------
#ifndef COMMON_H
#define COMMON_H
#include "SDL2/SDL.h"
#include "Descriptors.h"

class Common
{
	public:
        static std::string createReadableStringFromSection(section configSection);

        static std::string createReadableStringFromSection(vector<section> configSection);

		//set the static members "width" and "height"
		static void SetDimensions(const unsigned int width, const unsigned int height);
	
		//check if the ENTITY is within the bounds of "width" and "height"
		static bool TestBounds(Entity_desc &testMe, const bool invert);
		
		//check if the PIXEL is within the bounds of "width" and "height"
		static bool TestBounds(Pixel_desc &testMe, const bool invert);

		//check if float coordinated are within bounds
        static bool TestBounds(float &x, float &y, float &xVel, float &yVel, const bool invert);

        //check bounds using a CoOrd Struct (probably not useful)
        static bool TestBounds(CoOrd_fl &testMe, float &xVel, float &yVel, const bool invert);

		//decrease alpha to dim out a pixel
		static bool ApplyDimming(Pixel_desc &pix_t);

        static void RotatePoint(const float degrees, Pixel_desc &pix_t, const CoOrd center);

        static SDL_Surface* CreateSurface(int width, int height);

        static void surfaceToSurface(SDL_Surface* src, SDL_Surface* dest, int x_cent, int y_cent);

        static void surfaceToSurface(SDL_Surface* src,
                                     SDL_Rect src_rect,
                                     SDL_Surface* dest,
                                     int x_cent, int y_cent);

        static void PerPixel_surfaceToSurface(SDL_Surface *src,
                                                SDL_Rect src_rect,
                                                SDL_Surface* dest,
                                                int x_cent, int y_cent);

        static Uint32 getPixel(SDL_Surface* src, int x, int y);

        static Uint32 putPixel(SDL_Surface* dest, int x, int y);

	private:
		static unsigned int width, height;
};
#endif
