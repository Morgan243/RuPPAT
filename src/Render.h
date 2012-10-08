#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"
#include "Descriptors.h"

#ifndef RENDER_H
#define RENDER_H

class Render
{
	public:
		Render(int width, int height, int BPP, Uint32 flags);

		Render(int width, int height, int BPP, Uint32 flags,
			int game_width, int game_height);

		~Render();

		void setGameArea(int w, int h);

		void setMainScreen(int color);

		void OnRender();

		void OnRender(int x, int y);
		
		Uint32 getPixel(int x, int y, int screenID);

		void putPixel(int x, int y, Uint32 color, int screenID);
		
		void putSprite(int x, int y, Entity_desc* entityDetails);

		void putSprite(int x, int y, SDL_Surface* sprite);

		void putPixel2(int x, int y, Uint8 r, Uint8 g, Uint8 b);

		void applySurface(int x, int y, SDL_Surface* source);	
		
		SDL_Surface* mainScreen,* pre_surface, *sprite_surface;
	private:
		int mainWidth, game_width;
		int mainHeight, game_height;
		int  mainBPP;
		Uint32 mainFlags;
};

#endif
