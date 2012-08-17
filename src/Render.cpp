/*		Render 
 *
 *	this class contains the SDL screen itself as well as all the 
 * primary functions nescessary to actual draw pixels/sprites on it
 * mostly used by RuPPAT to display
 */

#include "Render.h"

//-----------------CONSTRUCTOR-----
//creates screen using arguments
Render::Render(int width, int height, int bpp, Uint32 flags)
{
	//assign private members for use later
	mainWidth = width;
	mainHeight = height;
	mainBPP = bpp;
	mainFlags = flags;

	//just init the sdl video stuff
	SDL_Init(SDL_INIT_VIDEO);

	//set caption
	SDL_WM_SetCaption("RuPPAT Demo",NULL);

	//finally, assign mainscreen to 
	mainScreen = SDL_SetVideoMode(width, height, bpp, flags);

  if ( mainScreen == NULL ) 
  {
    fprintf(stderr, "Unable to set video: %s\n", SDL_GetError());
    exit(1);
  }

}


//-----------------DECONSTRUCTOR------
Render::~Render()
{

SDL_Quit();

}

//-----------------setMainScreen-----
//this fills the main screen with the 
//color specified by the arg
void Render :: setMainScreen(int color)
{
SDL_FillRect(mainScreen, NULL, color);
}


//-----------------OnRender----------
//swap whats being shown with what has been added to main screen
void Render::OnRender()
{
//	SDL_FillRect(mainScreen, NULL, 0x000000);
	SDL_Flip(mainScreen);
//	SDL_UpdateRect(mainScreen, 0, 0, mainWidth, mainHeight);
}


//-----------------getPixel---------
//returns the color (32 bit unsigned int) of the
//pixel located at x, y args
Uint32 Render::getPixel(int x, int y, int screenID)
{
	Uint32 *pixels = (Uint32 *)mainScreen->pixels;
	return pixels[ ( y * mainScreen->w ) + x ];
}


//-----------------putPixel--------
//place a pixel of color and location as
//specified bu the arguments
void Render :: putPixel(int x, int y, int color, int screenID)
{
if (SDL_MUSTLOCK(mainScreen)) 
	SDL_LockSurface(mainScreen);	

	unsigned int *ptr = (unsigned int*)mainScreen->pixels;
	int lineoffset = y * (mainScreen->pitch / 4);
	

	int point = lineoffset + x;	

	//prevent placeing pixel outside of screens bounds
	if((point < mainHeight*mainWidth) && (point>=0))
		ptr[point] = color;


if( SDL_MUSTLOCK(mainScreen) )
        SDL_UnlockSurface(mainScreen);
}


//-----------------putPixel2--------
//a different way to putPixel, should act similar to putPixel
void Render :: putPixel2(int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
Uint32 *pixmem32;
    Uint32 colour;  
 
    colour = SDL_MapRGB( mainScreen->format, r, g, b );
  
    pixmem32 = (Uint32*) mainScreen->pixels  + y + x;
    *pixmem32 = colour;

}


//-----------------putSprite-------
//place sprite at position 
void Render :: putSprite(int x, int y, SDL_Surface* sprite)
{
	SDL_Rect imagePosition;

	imagePosition.x = x - (sprite->w/2);
	imagePosition.y = y - (sprite->h/2);
	
	imagePosition.w=0;
	imagePosition.h=0;

	 if(SDL_MUSTLOCK(mainScreen)){SDL_UnlockSurface(mainScreen);}
SDL_BlitSurface(sprite,NULL,mainScreen,&imagePosition);
}

//blit a surface onto the main screen, good for backgrounds and testing sprites
void Render::applySurface(int x, int y, SDL_Surface* source)
{
	//Need a rectangle to hold the offsets
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;
	offset.w = mainScreen->w;
	offset.h = mainScreen->h;
	

	//finally blit the source onto main at (x,y) on main
	SDL_BlitSurface(source,&offset,mainScreen,NULL);

}
