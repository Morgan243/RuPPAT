#include "math.h"
#include "Common.h"


unsigned int Common::width;
unsigned int Common::height;

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
