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
 //{{{
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

	//SDL_SetAlpha(mainScreen,0, 0xFF);
//	#if SDL_BYTEORDER == SDL_BIG_ENDIAN 
//		pre_surface = 
//		SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32, 0xFF000000, 
//		0x00FF0000, 0x0000FF00, 0x000000FF); 
//
//		sprite_surface = 
//		SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32, 0xFF000000, 
//		0x00FF0000, 0x0000FF00, 0x000000FF); 
//	#else 
//		sprite_surface = 
//		SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32, 0xFF000000, 
//		0x00FF0000, 0x0000FF00, 0x00000000); 
//
//		pre_surface = 
//		SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32, 0x000000FF, 
//		0x0000FF00, 0x00FF0000, 0xFF000000); 
//	#endif 


printf("Surface %s: w:%d h:%d bpp:%d\n", 
	"mainScreen", mainScreen->w, mainScreen->h, mainScreen->format->BitsPerPixel);

  if ( mainScreen == NULL ) 
  {
    fprintf(stderr, "Unable to set video: %s\n", SDL_GetError());
    exit(1);
  }
  //}}}
}

Render::Render(int width, int height, int bpp, Uint32 flags,
		int game_width, int game_height)
{
  //{{{
	//assign private members for use later
	mainWidth = width;
	mainHeight = height;
	mainBPP = bpp;
	mainFlags = flags;

	this->game_width = game_width;
	this->game_height = game_height;

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

		sprite_surface = 
		SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32, 0xFF000000, 
		0x00FF0000, 0x0000FF00, 0x000000FF); 
	#else 
		sprite_surface = 
		SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32, 0x000000FF, 
		0x0000FF00, 0x00FF0000, 0xFF000000); 

		pre_surface = 
		SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32, 0x000000FF, 
		0x0000FF00, 0x00FF0000, 0xFF000000); 
	#endif 


printf("Surface %s: w:%d h:%d bpp:%d\n", 
	"mainScreen", mainScreen->w, mainScreen->h, mainScreen->format->BitsPerPixel);

printf("Game space: width = %d, height = %d\n", game_width, game_height);

  if ( mainScreen == NULL ) 
  {
    fprintf(stderr, "Unable to set video: %s\n", SDL_GetError());
    exit(1);
  }
  //}}}
}

//-----------------DECONSTRUCTOR------
Render::~Render()
{
	SDL_FreeSurface(mainScreen);
	SDL_FreeSurface( pre_surface);
	SDL_Quit();
}

void Render::setGameArea(int w, int h)
{
	game_width = w;
	game_height = h;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN 
		pre_surface = 
		SDL_CreateRGBSurface(SDL_SWSURFACE,game_width,game_height,32, 0xFF000000, 
		0x00FF0000, 0x0000FF00, 0x000000FF); 

	#else 

		pre_surface = 
		SDL_CreateRGBSurface(SDL_SWSURFACE,game_width,game_height,32, 0x000000FF, 
		0x0000FF00, 0x00FF0000, 0xFF000000); 
	#endif 
		SDL_SetAlpha(pre_surface,SDL_SRCALPHA,0xFF);
		//SDL_DisplayFormatAlpha(pre_surface);
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
	//applySurface(0,0,sprite_surface);
	//pre_surface = rotozoomSurface(pre_surface, 0.0,1.0,0);
	applySurface(0,0,pre_surface);

	
	SDL_Flip(mainScreen);
	SDL_FillRect(pre_surface,NULL,0);
}

void Render::OnRender(int x, int y)
{
	//applySurface(0,0,sprite_surface);
	applySurface(x,y,pre_surface);

	
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
}

void Render::putPixel(vector<Pixel_desc> pixels)
{
	unsigned int color = 0;
	for(int i = 0; i < pixels.size(); i++)
	{
		//revers the endianess
		color = pixels[i].color;
		color = ((color&0x00ff0000)>>16)
				| ((color&0x0000ff00))
				| ((color&0x000000ff)<<16)
				| (color&0xff000000);

		putPixel(pixels[i].x, pixels[i].y,color, 0);

						
	}
}


//-----------------putPixel--------
//place a pixel of color and location as
//specified bu the arguments
void Render :: putPixel(int x, int y, Uint32 color, int screenID)
{
//{{{

	if (SDL_MUSTLOCK(pre_surface)) 
		SDL_LockSurface(pre_surface);	

		Uint8 r, g, b, a; 
		
		// This will probably warn you about addressing locals 
		SDL_GetRGBA(color, pre_surface->format, &r, &g, &b, &a); 
		 
		// Now give it transparent pixels 
		color = SDL_MapRGBA(pre_surface->format, r, g, b, a); 

		int bpp = pre_surface->format->BytesPerPixel;

		/* Here p is the address to the pixel we want to set */
		Uint8 *p = (Uint8 *)pre_surface->pixels + y * pre_surface->pitch + x * bpp;

		//prevent placeing pixel outside of screens bounds	
		if( ((y*(pre_surface->pitch/4)) < game_height*game_width) 
		    && (y*(pre_surface->pitch/4)) >=0 )
				*(Uint32 *)p = color;

	if( SDL_MUSTLOCK(pre_surface))
		SDL_UnlockSurface(pre_surface);
//}}}
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
//{{{
	SDL_Rect imagePosition;

	imagePosition.x = x - (sprite->w/2);
	imagePosition.y = y - (sprite->h/2);
	
	imagePosition.w=0;
	imagePosition.h=0;


	if(SDL_MUSTLOCK(pre_surface))
		SDL_LockSurface(pre_surface);

		///SDL_SetAlpha(sprite, 0, sprite->format->alpha);
		//finally blit the source onto main at (x,y) on main
		SDL_BlitSurface(sprite,NULL,pre_surface,&imagePosition);

		//SDL_SetAlpha(pre_surface, SDL_SRCALPHA , pre_surface->format->alpha);
		
	if(SDL_MUSTLOCK(pre_surface))
		SDL_UnlockSurface(pre_surface);

//}}}
}
//
//-----------------putSprite-------
//Put multiple sprites to the screen 
void Render :: putSprite(vector<Surface_Container> sprites)
{
	for(int i = 0; i < sprites.size(); i++)
	{
		putSprite(sprites[i].x, sprites[i].y, sprites[i].surface);
	}
}

//blit a surface onto the main screen, good for backgrounds and testing sprites
void Render::applySurface(int x, int y, SDL_Surface* source)
{
	//Need a rectangle to hold the offsets
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;
	offset.w = mainScreen->w + x;
	offset.h = mainScreen->h + y;
	
	if(SDL_MUSTLOCK(mainScreen))
		SDL_LockSurface(mainScreen);
//	if(SDL_MUSTLOCK(pre_surface))
//		SDL_LockSurface(pre_surface);

		//finally blit the source onto main at (x,y) on main
		SDL_BlitSurface(source,&offset,mainScreen,NULL);

//	if(SDL_MUSTLOCK(pre_surface))
//		SDL_UnlockSurface(pre_surface);
//
	if(SDL_MUSTLOCK(mainScreen))
		SDL_UnlockSurface(mainScreen);
}
