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
	mainWidth  = width;
	mainHeight = height;
	mainBPP    = bpp;
	mainFlags  = flags;

	//just init the sdl video stuff
	SDL_Init(SDL_INIT_VIDEO);

	//finally, assign mainscreen to 
    this->mainWindow = SDL_CreateWindow("RuPPAT Demo",
                                        //SDL_WINDOWPOS_UNDEFINED,
                                        //SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        width, height,
                                        0);

    //associate the main window with a new renderer
    this->renderer = SDL_CreateRenderer(this->mainWindow, -1,
                                        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    //create the primary streaming texture
    //this->mainTexture = SDL_CreateTexture(this->renderer,
    //                                      //SDL_PIXELFORMAT_RGBA8888,
    //                                      SDL_PIXELFORMAT_ABGR8888,
    //                                      SDL_TEXTUREACCESS_STREAMING,
    //                                    1920, 1080);

    //Set the clear screen color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    Uint32 r, g, b, a;
    if(!SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ARGB8888, &bpp,
                                    &r, &g, &b, &a))
    {
        printf("Pixel format thing failed!\n");
    }


    pre_surface = SDL_CreateRGBSurface(0, width, height, bpp, r, g, b, a);

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
//		SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32, 0x000000FF, 
//		0x0000FF00, 0x00FF0000, 0xFF000000); 
//
//		pre_surface = 
//		SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32, 0x000000FF, 
//		0x0000FF00, 0x00FF0000, 0xFF000000); 
//	#endif 

    sprite_surface = IMG_Load("small_red_circle.png");
    test_text = SDL_CreateTextureFromSurface(this->renderer, sprite_surface);
    //SDL_SetTextureBlendMode(mainTexture, SDL_BLENDMODE_BLEND);
    //SDL_SetSurfaceBlendMode(pre_surface, SDL_BLENDMODE_BLEND);
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

    this->mainWindow = SDL_CreateWindow("RuPPAT Demo",
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      width, height,
                                      SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);

    //associate the main window with a new renderer
    renderer = SDL_CreateRenderer(mainWindow, -1, 0);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

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
//		SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32, 0x000000FF, 
//		0x0000FF00, 0x00FF0000, 0xFF000000); 
//
//		pre_surface = 
//		SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32, 0x000000FF, 
//		0x0000FF00, 0x00FF0000, 0xFF000000); 
//	#endif 


//    printf("Surface %s: w:%d h:%d bpp:%d\n", 
//            "mainScreen", mainScreen->w, mainScreen->h, mainScreen->format->BitsPerPixel);
//
//    printf("Game space: width = %d, height = %d\n", game_width, game_height);
//
//    if ( mainScreen == NULL ) 
//    {
//        fprintf(stderr, "Unable to set video: %s\n", SDL_GetError());
//        exit(1);
//    }
  //}}}
}


//-----------------DECONSTRUCTOR------
Render::~Render()
{
	SDL_FreeSurface(mainScreen);
	SDL_FreeSurface( pre_surface);
	SDL_Quit();
}


void Render::setGameArea(const int w, const int h)
{
//{{{
	game_width = w;
	game_height = h;
//	#if SDL_BYTEORDER == SDL_BIG_ENDIAN 
//		pre_surface = 
//		SDL_CreateRGBSurface(SDL_SWSURFACE,game_width,game_height,32, 0xFF000000, 
//		0x00FF0000, 0x0000FF00, 0x000000FF); 
//
//	#else 
//
//		pre_surface = 
//		SDL_CreateRGBSurface(SDL_SWSURFACE,game_width,game_height,32, 0x000000FF, 
//		0x0000FF00, 0x00FF0000, 0xFF000000); 
//	#endif 
        
		//SDL_SetAlpha(pre_surface,SDL_SRCALPHA,0xFF);
//}}}
}


//-----------------setMainScreen-----
//this fills the main screen with the 
//color specified by the arg
void Render :: setMainScreen(const int color)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}


//-----------------OnRender----------
//swap whats being shown with what has been added to main screen
void Render::OnRender()
{
//{{{
    printf("RENDER!\n");
    SDL_UpdateTexture(this->mainTexture, NULL, pre_surface->pixels, pre_surface->pitch);
    SDL_RenderClear(this->renderer);
    SDL_RenderCopy(this->renderer, this->mainTexture, NULL, NULL);
    SDL_RenderPresent(this->renderer);


	//applySurface(0,0,pre_surface);
	
	//SDL_Flip(mainScreen);
	//SDL_FillRect(pre_surface,NULL,0);
//}}}
}


void Render::OnRender(const int x, const int y)
{
//{{{
	//applySurface(0,0,sprite_surface);
	//applySurface(x,y,pre_surface);

    xOrigin = x;
    yOrigin = y;
//
//
//    SDL_Rect location, sprite_rect;
//        location.h = this->mainHeight;
//        location.w = this->mainWidth;
//        location.x = x;
//        location.y = y;
//
//        sprite_rect.h = sprite_surface->h + 100;
//        sprite_rect.w = sprite_surface->w + 100;
//        sprite_rect.x = 190;
//        sprite_rect.y = 220;

//    for(comp_i = 0;  comp_i < this->low_composite.size(); comp_i++) 
//    {
//        SDL_RenderCopy(this->renderer, low_composite[comp_i], &location, NULL);
//    }

   // SDL_RenderCopy(this->renderer, this->mainTexture, &location, NULL);
//    SDL_RenderCopy(this->renderer, this->mainTexture, NULL, NULL);

    //SDL_RenderCopy(this->renderer, this->test_text, NULL, &sprite_rect);

//    comp_i = 0;
//    for(comp_i = 0;  comp_i < this->high_composite.size(); comp_i++) 
//    {
//        SDL_RenderCopy(this->renderer, high_composite[comp_i], &location, NULL);
//    }

    SDL_RenderPresent(this->renderer);
    //SDL_UpdateTexture(this->mainTexture, NULL, pre_surface->pixels, pre_surface->pitch);
    //this->clearMainText();
	
	//SDL_Flip(mainScreen);
	//SDL_FillRect(pre_surface,NULL,0);
//}}}
}

//-----------------getPixel---------
//returns the color (32 bit unsigned int) of the
//pixel located at x, y args
Uint32 Render::getPixel(const int x, const int y, int screenID) const
{
//{{{
	Uint32 *pixels = (Uint32 *)pre_surface->pixels;
	return pixels[ ( y * pre_surface->w ) + x ];
//}}}
}


void Render::putPixel(const vector<Pixel_desc> pixels)
{
//{{{
	unsigned int color = 0;
	for(int i = 0; i < pixels.size(); i++)
	{
		//revers the endianess
		color = pixels[i].color;
		color = ((color&0x00ff0000)>>16)
				| ((color&0x0000ff00))
				| ((color&0x000000ff)<<16)
				| (color&0xff000000);

		putPixel(pixels[i].x, pixels[i].y, color, 0);
	}
//}}}
}


//-----------------putPixel--------
//place a pixel of color and location as
//specified bu the arguments
void Render :: putPixel(const int x, const int y, 
                        Uint32 color, const int screenID)
{
//{{{


        SDL_SetTextureAlphaMod(test_text, (color & 0xff000000)>>24);
        putSprite(x,y,test_text);

//	if (SDL_MUSTLOCK(pre_surface)) 
//		SDL_LockSurface(pre_surface);	
//
//		Uint8 r, g, b, a; 
//        int access, w, h;
//        SDL_QueryTexture (this->mainTexture, &this->main_format, &access, &w, &h);
////		
////		// This will probably warn you about addressing locals 
//		SDL_GetRGBA(color, pre_surface->format, &r, &g, &b, &a); 
////		 
////		// Now give it transparent pixels 
//		color = SDL_MapRGBA(pre_surface->format, r, g, b, a); 
////
//		int bpp = pre_surface->format->BytesPerPixel;
//
//		/* Here p is the address to the pixel we want to set */
//		Uint8 *p = (Uint8 *)pre_surface->pixels + y * pre_surface->pitch + x * bpp;
//
//		//prevent placeing pixel outside of screens bounds	
//		if( ((y*(pre_surface->pitch/4)) < game_height*game_width) 
//		    && (y*(pre_surface->pitch/4)) >=0 )
//				*(Uint32 *)p = color;
//
//	if( SDL_MUSTLOCK(pre_surface))
//		SDL_UnlockSurface(pre_surface);

//    void *mainPix;
//    int pitch;
//    SDL_Rect lock_area;
//        lock_area.x=x;
//        lock_area.y=y;
//        lock_area.w=1;
//        lock_area.h=1;
//
//    if(SDL_LockTexture(this->mainTexture, &lock_area, &mainPix, &pitch))
//        printf("ERROR LOCKING TEXTURE!\n");
//
//		Uint8 *p = (Uint8 *)mainPix + y * pitch + x * bpp;
//
//		//prevent placeing pixel outside of screens bounds	
//		if( ((y*(pitch/4)) < game_height*game_width) 
//		    && (y*(pitch/4)) >=0 )
//				*(Uint32 *)p = color;
//
//    SDL_UnlockTexture(this->mainTexture);
//}}}
}


//-----------------putSprite-------
//place sprite at position 
void Render :: putSprite(const int x, const int y, SDL_Surface* sprite)
{
//{{{
	SDL_Rect imagePosition;

	imagePosition.x = x - (sprite->w/2);
	imagePosition.y = y - (sprite->h/2);
	
	imagePosition.w = sprite->w;
	imagePosition.h = sprite->h;


	if(SDL_MUSTLOCK(pre_surface))
		SDL_LockSurface(pre_surface);

		//finally blit the source onto main at (x,y) on main
		//SDL_BlitSurface(sprite,NULL,pre_surface,&imagePosition);
        SDL_UpdateTexture(this->mainTexture, &imagePosition, sprite->pixels, sprite->pitch);

	if(SDL_MUSTLOCK(pre_surface))
		SDL_UnlockSurface(pre_surface);
//}}}
}


//-----------------putSprite-------
//Put multiple sprites to the screen 
void Render :: putSprite(const vector<Surface_Container> sprites)
{
//{{{
	for(int i = 0; i < sprites.size(); i++)
	{
		putSprite(sprites[i].x, sprites[i].y, sprites[i].surface);
	}
//}}}
}

void Render::putSprite(const int x,
                const int y,
                SDL_Texture* sprite)
{
//{{{
    SDL_Rect location;
        SDL_QueryTexture(sprite, NULL, NULL, &location.w, &location.h);

        //cout<<"w , h: "<<location.w<<", "<<location.h<<endl;

        location.x = x - (location.w/2);
        location.y = y - (location.h/2);
        location.x -= xOrigin;
        location.y -= yOrigin;

        //location.w = this->mainWidth;
        //location.h = this->mainHeight;
        //location.w = 100;
        //location.h = 100;
//    if( ((location.x - location.w)> 0) && ((location.x +location.w) < mainWidth)
//            && ((location.y - location.h) > 0) && ((location.y + location.h) < mainHeight))

        SDL_RenderCopy(this->renderer, sprite, NULL, &location);
//}}}
}

void Render::putSprite(const vector<Texture_Container> sprites)
{
    //{{{
    for(int i = 0; i < sprites.size(); i++)
    {
        putSprite(sprites.at(i).x, sprites.at(i).y, sprites.at(i).texture);
    }
    //}}}
}


//blit a surface onto the main screen, good for backgrounds and testing sprites
void Render::applySurface(const int x, const int y, SDL_Surface* source)
{
//{{{
	//Need a rectangle to hold the offsets
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;
	offset.w = mainScreen->w + x;
	offset.h = mainScreen->h + y;
	
	if(SDL_MUSTLOCK(mainScreen))
		SDL_LockSurface(mainScreen);

		//finally blit the source onto main at (x,y) on main
		SDL_BlitSurface(source,&offset,mainScreen,NULL);

	if(SDL_MUSTLOCK(mainScreen))
		SDL_UnlockSurface(mainScreen);
//}}}
}

void Render::applyTextureToMain(const int x,
                        const int y,
                        SDL_Texture* source)
{
    //{{{
    SDL_Rect location;
        location.x = x;
        location.y = y;
        location.w = this->mainWidth;
        location.h = this->mainHeight;
    SDL_RenderCopy(this->renderer, source, &location, NULL);
    //SDL_RenderPresent(this->renderer);
    //}}}
}

void Render::clearMainText()
{
//{{{
    void *mainPix;
    int pitch;
    SDL_Rect lock_area;
        //lock_area.x=x;
        //lock_area.y=y;
        lock_area.w=1;
        lock_area.h=1;

    if(SDL_LockTexture(this->mainTexture, NULL, &mainPix, &pitch))
        printf("ERROR LOCKING TEXTURE!\n");

		//Uint8 *p = (Uint8 *)mainPix + y * pitch + x * bpp;
        memset(mainPix, getPixel(1,1,0), sizeof(Uint32)*mainWidth*mainHeight);
		//prevent placeing pixel outside of screens bounds	
		//if( ((y*(pitch/4)) < game_height*game_width) 
		//    && (y*(pitch/4)) >=0 )
		//		*(Uint32 *)p = 0xffffffff;

    SDL_UnlockTexture(this->mainTexture);
//}}}
}
