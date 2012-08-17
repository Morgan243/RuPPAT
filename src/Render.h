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
	~Render();

	void setMainScreen(int color);

	void OnRender();
	
	Uint32 getPixel(int x, int y, int screenID);

	void putPixel(int x, int y, int color, int screenID);
	
	void putSprite(int x, int y, Entity_desc* entityDetails);

	void putSprite(int x, int y, SDL_Surface* sprite);

	void putPixel2(int x, int y, Uint8 r, Uint8 g, Uint8 b);

	void applySurface(int x, int y, SDL_Surface* source);	
	
	SDL_Surface* mainScreen;
private:
	
	

	int mainWidth;
	int mainHeight;
	int  mainBPP;
	Uint32 mainFlags;
};

#endif
