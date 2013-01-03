#include "Common.h"


unsigned int Common::width;
unsigned int Common::height;

void Common::SetDimensions(unsigned int wid, unsigned int hei)
{
//{{{
	Common::width = wid;
	Common::height = hei;
//}}}
}

bool Common::TestBounds(Entity_desc &testMe, bool invert)
{
//{{{
	//make sure the new location is within bounds
	if(invert)
	{
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
	}
	else
	{
		if(testMe.x<=0)
			return true;

		if(testMe.x>=width)
			return true;

		if(testMe.y<=0)
			return true;

		if(testMe.y>=height)
			return true;

	}
//}}}
}

bool Common::TestBounds(Pixel_desc &testMe, bool invert)
{
//{{{
	//make sure the new location is within bounds
	if(invert)
	{
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
	}
	else
	{
		if(testMe.x<=0)
			return true;

		if(testMe.x>=width)
			return true;

		if(testMe.y<=0)
			return true;

		if(testMe.y>=height)
			return true;

	}
//}}}
}

bool Common::ApplyDimming(Pixel_desc &pix_t)
{
//{{{
	const int dim_scale = 1; //bring this up to increase max dim length
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
