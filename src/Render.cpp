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

#if SDL_BYTEORDER == SDL_BIG_ENDIAN 
pre_surface = 
SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32, 0xFF000000, 
0x00FF0000, 0x0000FF00, 0x000000FF); 
#else 
pre_surface = 
SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32, 0x000000FF, 
0x0000FF00, 0x00FF0000, 0xFF000000); 
#endif 


printf("Surface %s: w:%d h:%d bpp:%d\n", 
	"mainScreen", mainScreen->w, mainScreen->h, mainScreen->format->BitsPerPixel);

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
//	putSprite(0,0,pre_surface);
	
	applySurface(0,0,pre_surface);
	SDL_Flip(mainScreen);
	SDL_FillRect(pre_surface,NULL,0);
}


//-----------------getPixel---------
//returns the color (32 bit unsigned int) of the
//pixel located at x, y args
Uint32 Render::getPixel(int x, int y, int screenID)
{

	Uint32 *pixels = (Uint32 *)pre_surface->pixels;
	return pixels[ ( y * pre_surface->w ) + x ];

//	Uint32 *pixels = (Uint32 *)mainScreen->pixels;
//	return pixels[ ( y * mainScreen->w ) + x ];
}


//-----------------putPixel--------
//place a pixel of color and location as
//specified bu the arguments
void Render :: putPixel(int x, int y, Uint32 color, int screenID)
{
//if (SDL_MUSTLOCK(mainScreen)) 
//	SDL_LockSurface(mainScreen);	
//
//Uint8 r, g, b, a; 
//// This will probably warn you about addressing locals 
// SDL_GetRGBA(color, mainScreen->format, &r, &g, &b, &a); 
//
// // Now give it transparent pixels 
// color = SDL_MapRGBA(mainScreen->format, r, g, b, SDL_ALPHA_TRANSPARENT); 
//
//
//    int bpp = mainScreen->format->BytesPerPixel;
//    /* Here p is the address to the pixel we want to set */
//    Uint8 *p = (Uint8 *)mainScreen->pixels + y * mainScreen->pitch + x * bpp;
//	
//	*(Uint32 *)p = color;
//
//if( SDL_MUSTLOCK(mainScreen) )
//        SDL_UnlockSurface(mainScreen);
//

if (SDL_MUSTLOCK(pre_surface)) 
	SDL_LockSurface(pre_surface);	

Uint8 r, g, b, a; 
// This will probably warn you about addressing locals 
 SDL_GetRGBA(color, pre_surface->format, &r, &g, &b, &a); 
 
 // Now give it transparent pixels 
 color = SDL_MapRGBA(pre_surface->format, r, g, b, a); 
//std::cout<<"Pixel color: "<<color<<std::endl;

    int bpp = pre_surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)pre_surface->pixels + y * pre_surface->pitch + x * bpp;

	//prevent placeing pixel outside of screens bounds
	
if( ((y*(pre_surface->pitch/4)) < mainHeight*mainWidth) && (y*(pre_surface->pitch/4)) >=0)	
		*(Uint32 *)p = color;

if( SDL_MUSTLOCK(pre_surface))
        SDL_UnlockSurface(pre_surface);
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

	 if(SDL_MUSTLOCK(mainScreen))
		SDL_LockSurface(mainScreen);

		SDL_BlitSurface(sprite,NULL,mainScreen,&imagePosition);
	
	if(SDL_MUSTLOCK(mainScreen))
		SDL_UnlockSurface(mainScreen);
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
	
	if(SDL_MUSTLOCK(mainScreen))
		SDL_LockSurface(mainScreen);

		//finally blit the source onto main at (x,y) on main
		SDL_BlitSurface(source,&offset,mainScreen,NULL);

	if(SDL_MUSTLOCK(mainScreen))
		SDL_UnlockSurface(mainScreen);
}
