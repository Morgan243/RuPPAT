// 			[Class] Render
//--------------------------------------------------------------------
//| 	Methods and surfaces of the primary screens/buffers. Everything
//|that ends up on the screen eventually must go through one of these
//|methods.
//--------------------------------------------------------------------
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
		Render(int width,
			   	int height,
			   	int BPP,
			   	Uint32 flags);

		Render(int width,
			   	int height,
			   	int BPP,
			   	Uint32 flags,
				int game_width,
			   	int game_height);

		~Render();

		void setGameArea(const int w, const int h);

		void setMainScreen(const int color);

		void OnRender();

		void OnRender(const int x, const int y);
		
		const Uint32 getPixel(const int x,
                                const int y,
                                const int screenID);

		void putPixel(const vector<Pixel_desc> pixels);

		void putPixel(const int x,
			   			const int y,
					   	Uint32 color,
					   	const int screenID);

		void putSprite(const int x,
			   			const int y,
					   	SDL_Surface* sprite);

		void putSprite(const vector<Surface_Container> sprites);

		void applySurface(const int x,
			   				const int y,
						   	SDL_Surface* source);	
		
		SDL_Surface *mainScreen,
					*pre_surface,
				   	*sprite_surface;

	private:
		int mainWidth, game_width,
			 mainHeight, game_height,
			 mainBPP;

		Uint32 mainFlags;
};
#endif
