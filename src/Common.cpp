#include "math.h"
#include "Common.h"


unsigned int Common::width;
unsigned int Common::height;

using namespace std;

string Common::createReadableStringFromSection(section configSection)
{
//{{{
    string readable = "Section Title: " + configSection.title + "\n";

    for(int i = 0; i < configSection.sectionOptions.size(); i++)
    {
        readable += "\t Option:: " + configSection.sectionOptions.at(i).option + "\n";

        for(int j = 0; j < configSection.sectionOptions.at(i).values.size(); j++)
        {
            readable += "\t  --" + configSection.sectionOptions.at(i).values.at(j) + "\n";
        }
    }

    return readable;
    //}}}
}

string Common::createReadableStringFromSection(vector<section> configSection)
{
//{{{
    string readable = "";

    for(int i = 0; i < configSection.size(); i++)
    {
        readable += createReadableStringFromSection(configSection.at(i));
    }

    return readable;
//}}}
}

void Common::SetDimensions(const unsigned int wid, const unsigned int hei) 
{
//{{{
	Common::width = wid;
	Common::height = hei;
//}}}
}

bool Common::TestBounds(Entity_desc &testMe, const bool invert)
{
//{{{
	//make sure the new location is within bounds
	if(invert)
	{
    //{{{
		bool isOutOfBounds = false;

        //x left region boundry
		if(testMe.x<=0)
			{
				testMe.x=1;
				testMe.xVel=testMe.xVel*(-1);
				isOutOfBounds=true;
			}

        //x right boundry
        else if(testMe.x>=width)
			{
				testMe.x=width-1;
				testMe.xVel=testMe.xVel*(-1);
				isOutOfBounds=true;
			}

        //y left boundry
		if(testMe.y<=0)
			{
				testMe.y=1;
				testMe.yVel=testMe.yVel*(-1);
				isOutOfBounds=true;
			}

        //y right boundry check
        else if(testMe.y>=height)
			{
				testMe.y=height-1;
				testMe.yVel=testMe.yVel*(-1);
				isOutOfBounds=true;
			}	

		return isOutOfBounds;
    //}}}
	}
	else
	{
    //{{{
		if(testMe.x<=0)
			return true;

		if(testMe.x>=width)
			return true;

		if(testMe.y<=0)
			return true;

		if(testMe.y>=height)
			return true;
    //}}}
	}
//}}}
}

bool Common::TestBounds(Pixel_desc &testMe, const bool invert)
{
//{{{
	//make sure the new location is within bounds
	if(invert)
	{
    //{{{
		bool isOutOfBounds = false;

		if(testMe.x<=0)
			{
				testMe.x=1;
				testMe.xVel=testMe.xVel*(-1);
				isOutOfBounds=true;
			}

		if(testMe.x>=width)
			{
				testMe.x=width-1;
				testMe.xVel=testMe.xVel*(-1);
				isOutOfBounds=true;
			}

		if(testMe.y<=0)
			{
				testMe.y=1;
				testMe.yVel=testMe.yVel*(-1);
				isOutOfBounds=true;
			}

		if(testMe.y>=height)
			{
				testMe.y=height-1;
				testMe.yVel=testMe.yVel*(-1);
				isOutOfBounds=true;
			}	

		return isOutOfBounds;
    //}}}
	}
	else
	{
    //{{{
		if(testMe.x<=0)
			return true;

		if(testMe.x>=width)
			return true;

		if(testMe.y<=0)
			return true;

		if(testMe.y>=height)
			return true;
    //}}}
	}
//}}}
}

bool Common::TestBounds(float &x, float &y, float &xVel, float &yVel, const bool invert)
{
//{{{
	bool isOutOfBounds = false;

	//make sure the new location is within bounds
	if(invert)
	{
    //{{{
        if(x<=0)
            {
                x=1;
                xVel = xVel*(-1);
                isOutOfBounds = true;
            }

        if(x>=width)
            {
                x = width-1;
                xVel = xVel*(-1);
                isOutOfBounds = true;
            }

        if(y<=0)
            {
                y = 1;
                yVel = yVel*(-1);
                isOutOfBounds = true;
            }

        if(y>=height)
            {
                y = height-1;
                yVel = yVel*(-1);
                isOutOfBounds = true;
            }	
    //}}}
	}
	else
	{
    //{{{
        if(x<=0)
                isOutOfBounds = true;

        if(x>=width)
                isOutOfBounds = true;

        if(y<=0)
                isOutOfBounds = true;

        if(y>=height)
                isOutOfBounds = true;
    //}}}
	}
//}}}

    return isOutOfBounds;
}

bool Common::TestBounds(CoOrd_fl &testMe, float &xVel, float &yVel, const bool invert)
{
//{{{
	bool isOutOfBounds = false;

	//make sure the new location is within bounds
	if(invert)
	{
    //{{{
        if(testMe.x<=0)
            {
                testMe.x=1;
                xVel = xVel*(-1);
                isOutOfBounds = true;
            }

        if(testMe.x>=width)
            {
                testMe.x = width-1;
                xVel = xVel*(-1);
                isOutOfBounds = true;
            }

        if(testMe.y<=0)
            {
                testMe.y = 1;
                yVel = yVel*(-1);
                isOutOfBounds = true;
            }

        if(testMe.y>=height)
            {
                testMe.y = height-1;
                yVel = yVel*(-1);
                isOutOfBounds = true;
            }	
    //}}}
	}
	else
	{
    //{{{
        if(testMe.x<=0)
                isOutOfBounds = true;

        if(testMe.x>=width)
                isOutOfBounds = true;

        if(testMe.y<=0)
                isOutOfBounds = true;

        if(testMe.y>=height)
                isOutOfBounds = true;
    //}}}
	}

    return isOutOfBounds;
//}}}
}

bool Common::ApplyDimming(Pixel_desc &pix_t)
{
//{{{
    //bring this up to increase max dim length
	const int dim_scale = 1; 

	int dimFactor = pix_t.dimFactor;
	int current_color = pix_t.color;
	unsigned int dimTimer= pix_t.dimTimer;
	unsigned int tmpA; 
	
	//if pixel is to be dimmed and dimTimer is over factor
	if(dimFactor &&  (++dimTimer>dimFactor))
	{
		//reset the dimtimer
		dimTimer = 0;

		tmpA= (Uint32)((current_color & 0xFF000000)>>24);
		tmpA -=5;

		//recombine the colors
		//pix_t.color = ( (tmpA<<24) | (tmpR<<16) | (tmpG<<8) | tmpB);
		pix_t.color = ( (tmpA<<24 & 0xFF000000) | (0x00FFFFFF&current_color));
	}

	//reassign timer
	pix_t.dimTimer = dimTimer;

	//if the color is really dark, just remove the pixel
	if(tmpA<10)
		{
			pix_t.deleteMe = true;
		}
//}}}
}

void Common::RotatePoint(const float degrees, Pixel_desc &pix_t, const CoOrd center)
{
//{{{
	int tempX = pix_t.x, tempY = pix_t.y; 
	float theta, mag;

	//add width and height
	tempY -= center.y;
	tempX -= center.x;

    tempY *= (-1);

    if(tempX)
        theta = atan( ((float)tempY) / ((float)tempX) );
    else
    {
        if( tempY > 0)
            theta = 3.14159/2.0;
        else if (tempY<0)
            theta = (3.14159/2.0)*3.0;
        else
            theta = 0;
    }

    mag = sqrt((float)(tempX*tempX) + (float)(tempY*tempY));

    if(theta == -0 || theta == +0)
        theta = 0;

		//find initial theta
		if(tempY>0)//first or second quadrant
		{
			if(tempX>0)//first quad
			{
				//do nothing
			}
			else//second quad
			{
				//theta = theta*(-1.0) + 90.0*0.01745329251;
				theta = 180.0*0.01745329251 - (theta * -1.0);
			}
		}
		else//third or fourth quad
		{
			if(tempX>0)
			{
				//theta = theta*(-1.0) + 270.0*0.01745329251;
			}
			else
			{
			    theta = theta + 180.0*0.01745329251;
			}
		}

    //convert degrees to radians
    theta += (degrees*0.01745329251);

    tempY = sin(theta)*mag;
    tempX = cos(theta)*mag;

    //invert Y
    tempY *= (-1.0);

    pix_t.x = tempX;
    pix_t.y = tempY;
//}}}
}


SDL_Surface* Common::CreateSurface(int w, int h)
{
//{{{
//	#if SDL_BYTEORDER == SDL_BIG_ENDIAN 
//		return SDL_CreateRGBSurface(SDL_SWSURFACE |SDL_SRCALPHA,w,h,32, 0xFF000000, 
//		0x00FF0000, 0x0000FF00, 0x000000FF); 
//	#else 
//		return SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA,w,h,32, 0x000000FF, 
//		0x0000FF00, 0x00FF0000, 0xFF000000); 
//	#endif 
//}}}
}


void Common::surfaceToSurface(SDL_Surface* src, SDL_Surface* dest, int x_cent, int y_cent)
{
//{{{
    SDL_Rect position;

    position.x = x_cent - (src->w/2);
    position.y = y_cent - (src->h/2);

    position.w = 0;
    position.h = 0;

    if(SDL_MUSTLOCK(dest))
        SDL_LockSurface(dest);

		SDL_BlitSurface(src, NULL, dest, &position);

    if(SDL_MUSTLOCK(dest))
        SDL_UnlockSurface(dest);
//}}}
}

void Common::surfaceToSurface(SDL_Surface* src,
                             SDL_Rect src_rect,
                             SDL_Surface* dest,
                             int x_cent, int y_cent)
{
//{{{
    SDL_Rect position;

    position.x = x_cent;//x_cent - (src_rect.w/2);
    position.y = y_cent;//y_cent - (src_rect.h/2);

    position.w = 0;
    position.h = 0;

    if(SDL_MUSTLOCK(dest))
        SDL_LockSurface(dest);

		SDL_BlitSurface(src, &src_rect, dest, &position);

    if(SDL_MUSTLOCK(dest))
        SDL_UnlockSurface(dest);
//}}}
}

void Common::PerPixel_surfaceToSurface(SDL_Surface *src,
                                            SDL_Rect src_rect,
                                            SDL_Surface* dest,
                                            int x_cent, int y_cent)
{
//{{{
//}}}
}

Uint32 Common::getPixel(SDL_Surface* src, int x, int y)
{
//{{{

//}}}
}

Uint32 Common::putPixel(SDL_Surface* dest, int x, int y)
{
//{{{
//	if (SDL_MUSTLOCK(dest)) 
//		SDL_LockSurface(dest);	
//
//		Uint8 r, g, b, a, color; 
//		
//		// This will probably warn you about addressing locals 
//		SDL_GetRGBA(color, dest->format, &r, &g, &b, &a); 
//		 
//		// Now give it transparent pixels 
//		color = SDL_MapRGBA(dest->format, r, g, b, a); 
//
//		int bpp = dest->format->BytesPerPixel;
//
//		/* Here p is the address to the pixel we want to set */
//		Uint8 *p = (Uint8 *)dest->pixels + y * dest->pitch + x * bpp;
//
//		//prevent placeing pixel outside of screens bounds	
//		if( ((y*(dest->pitch/4)) < game_height*game_width) 
//		    && (y*(->pitch/4)) >=0 )
//				*(Uint32 *)p = color;
//
//	if( SDL_MUSTLOCK(dest))
//		SDL_UnlockSurface(dest);
//}}}
}
