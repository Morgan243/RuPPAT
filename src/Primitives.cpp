#include "Primitives.h"
#include "math.h"

Primitives::Primitives()
{

}

Primitives::Primitives(int def_lnWidth, Uint32 def_lnColor, SDL_Surface *surface)
{
	this->default_lineWidth = def_lnWidth;

	this->default_lineColor = def_lnColor;

	this->default_surface = surface;
}

Primitives::~Primitives()
{
	if(default_surface != NULL)
		SDL_FreeSurface(default_surface);

	for(int i = 0; i < surface_cache.size() ; i++)
		delete(surface_cache[i]);
}

float Primitives::calcSlope(CoOrd pointA, CoOrd pointB)
{
	if((pointA.x - pointB.x)  != 0)
	return ((float)pointA.y - (float)pointB.y)/((float)pointA.x - (float)pointB.x);
}

int Primitives::drawCircle(CoOrd center, int radius, bool cache)
{
//{{{
	int x = 0, y =0;
	float theta = 0.0, i = 0.0;
	Uint32 tempColor = default_lineColor;
	if(!cache)
	{
		for(;radius >15 ; radius--)
		{
			for(theta=0.0; theta<360.0; theta+=0.1)
			{
				
				x = (radius * cos(theta)) + center.x;
				y = (radius * sin(theta)) + center.y;
				//cout<<"X,Y:"<<x<<","<<y<<endl;
				putPixel(x,y,tempColor, default_surface);
			}
			tempColor = (tempColor & 0xFFFFFF00) | ((tempColor & 0x000000FF)-15);
		}
		return -1;
	}
	else
	{
		center.x = radius;
		center.y = radius;
		Surface_Container *tempContain = new Surface_Container;

		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		tempContain->surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
							radius*2 ,radius*2,32, 
							0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF); 
				cout<<"BIG ENDIAN"<<endl;
		#else

		tempContain->surface =
				SDL_CreateRGBSurface(SDL_SWSURFACE,radius*2,radius*2,32, 0x000000FF, 
										0x0000FF00, 0x00FF0000, 0xFF000000); 
				cout<<"Littel Endian! Color: "<<default_lineColor<<endl;
		
		#endif

		for(;radius >14 ; radius--)
		{
			for(theta=0.0; theta<360.0; theta+=0.1)
			{
				tempColor = (default_lineColor & 0x00FFFFff) |
						(((unsigned int)(default_lineColor & 0xff000000)-
							(unsigned int)(195*cos(theta))&0xff000000));
				x = (radius * cos(theta)) + center.x;
				y = (radius * sin(theta)) + center.y;

		//		cout<<"X,Y:"<<x<<","<<y<<" with color:"<<tempColor<<endl;
				putPixel(x,y,tempColor, tempContain->surface);
			}
			///tempColor = (tempColor & 0xFFFFFF00) | ((tempColor & 0x000000FF)-15);
		}
		tempContain->ID = surface_cache.size();
	       	
		
		surface_cache.push_back(tempContain);	
		
		tempContain->surface = SDL_DisplayFormatAlpha(tempContain->surface);
		tempContain->surface = rotozoomSurface(tempContain->surface,
						0.0, 1.0,0);

		SDL_SetAlpha(tempContain->surface,0,0xff);
		return surface_cache.size()-1;
	}
//}}}
}

void Primitives::drawCircle(CoOrd center, int radius, SDL_Surface *surface)
{

}

void Primitives::drawLine(CoOrd pointA, CoOrd pointB)
{
	//{{{
	float slope = calcSlope(pointA, pointB);
	float yIntercept = (float)(pointA.y)-(slope * (float) pointA.x);
	int temp_y = 0; 

	if( pointB.x > pointA.x)
	{
		if( pointB.y > pointA.y)
		{
			//keep going as long as x and y values havent exceeded pointB
			for(int i = pointA.x; (temp_y < pointB.y) || (i < pointB.x) ;i++)	
			{
				temp_y = slope*i + yIntercept;// + j;
				putPixel(i, temp_y, default_lineColor, default_surface);
			}
		}
		else if( pointB.y < pointA.y)
		{
			//keep going as long as x and y values havent exceeded pointB
			for(int i = pointA.x; (temp_y > pointB.y) || (i < pointB.x) ;i++)	
			{
				temp_y = slope*i + yIntercept;// + j;
				putPixel(i, temp_y, default_lineColor, default_surface);
			}
		}
		else if( pointB.y == pointA.y)
		{
			temp_y = pointB.y;

			//keep going as long as x and y values havent exceeded pointB
			for(int i = pointA.x; (i < pointB.x) ;i++)	
			{
				putPixel(i, temp_y, default_lineColor, default_surface);
			}
		}
	}
	else if( pointB.x < pointA.x)
	{
		if( pointB.y > pointA.y)
		{
			//keep going as long as x and y values havent exceeded pointB
			for(int i = pointA.x; (temp_y < pointB.y) || (i > pointB.x) ;i--)	
			{
				temp_y = slope*i + yIntercept;// + j;
				putPixel(i, temp_y, default_lineColor, default_surface);
			}
		}
		else if( pointB.y < pointA.y)
		{
			//keep going as long as x and y values havent exceeded pointB
			for(int i = pointA.x; (temp_y > pointB.y) || (i > pointB.x) ;i--)	
			{
				temp_y = slope*i + yIntercept;// + j;
				putPixel(i, temp_y, default_lineColor, default_surface);
			}
		}
		else if( pointB.y == pointA.y)
		{
			temp_y = pointB.y;

			//keep going as long as x and y values havent exceeded pointB
			for(int i = pointA.x; (i > pointB.x) ;i--)	
			{
				putPixel(i, temp_y, default_lineColor, default_surface);
			}
		}
	}
	else if( pointB.x == pointA.x)
	{
		if( pointB.y > pointA.y)
		{
			//keep going as long as y values havent exceeded pointB
			for(int i = pointA.y; (i < pointB.y) ;i++)	
			{
				putPixel(pointB.x, i, default_lineColor, default_surface);
			}
		}
		else if( pointB.y < pointA.y)
		{
			//keep going as long as y values havent exceeded pointB
			for(int i = pointA.y; (i > pointB.y); i--)	
			{
				putPixel(pointB.x, i, default_lineColor, default_surface);
			}
		}


	}
//}}}
}

void Primitives::drawLine(CoOrd pointA, CoOrd pointB, SDL_Surface *surface)
{
	//{{{
	float slope = calcSlope(pointA, pointB);
	float yIntercept = (float)(pointA.y)-(slope * (float) pointA.x);
	int temp_y = 0; 

	if( pointB.x > pointA.x)
	{
		if( pointB.y > pointA.y)
		{
			//keep going as long as x and y values havent exceeded pointB
			for(int i = pointA.x; (temp_y < pointB.y) || (i < pointB.x) ;i++)	
			{
				temp_y = slope*i + yIntercept;// + j;
				putPixel(i, temp_y, default_lineColor, surface);
			}
		}
		else if( pointB.y < pointA.y)
		{
			//keep going as long as x and y values havent exceeded pointB
			for(int i = pointA.x; (temp_y > pointB.y) || (i < pointB.x) ;i++)	
			{
				temp_y = slope*i + yIntercept;// + j;
				putPixel(i, temp_y, default_lineColor, surface);
			}
		}
		else if( pointB.y == pointA.y)
		{
			temp_y = pointB.y;

			//keep going as long as x and y values havent exceeded pointB
			for(int i = pointA.x; (i < pointB.x) ;i++)	
			{
				putPixel(i, temp_y, default_lineColor, surface);
			}
		}
	}
	else if( pointB.x < pointA.x)
	{
		if( pointB.y > pointA.y)
		{
			//keep going as long as x and y values havent exceeded pointB
			for(int i = pointA.x; (temp_y < pointB.y) || (i > pointB.x) ;i--)	
			{
				temp_y = slope*i + yIntercept;// + j;
				putPixel(i, temp_y, default_lineColor, surface);
			}
		}
		else if( pointB.y < pointA.y)
		{
			//keep going as long as x and y values havent exceeded pointB
			for(int i = pointA.x; (temp_y > pointB.y) || (i > pointB.x) ;i--)	
			{
				temp_y = slope*i + yIntercept;// + j;
				putPixel(i, temp_y, default_lineColor, surface);
			}
		}
		else if( pointB.y == pointA.y)
		{
			temp_y = pointB.y;

			//keep going as long as x and y values havent exceeded pointB
			for(int i = pointA.x; (i > pointB.x) ;i--)	
			{
				putPixel(i, temp_y, default_lineColor, surface);
			}
		}
	}
	else if( pointB.x == pointA.x)
	{
		if( pointB.y > pointA.y)
		{
			//keep going as long as y values havent exceeded pointB
			for(int i = pointA.y; (i < pointB.y) ;i++)	
			{
				putPixel(pointB.x, i, default_lineColor, surface);
			}
		}
		else if( pointB.y < pointA.y)
		{
			//keep going as long as y values havent exceeded pointB
			for(int i = pointA.y; (i > pointB.y); i--)	
			{
				putPixel(pointB.x, i, default_lineColor, surface);
			}
		}


	}
//}}}
}

void Primitives::drawLine(CoOrd pointA, CoOrd pointB, int width)
{
	//{{{
	float slope = calcSlope(pointA, pointB);
	float yIntercept = (float)(pointA.y)-(slope * (float) pointA.x);
	int temp_y = 0; 

	if( pointB.x > pointA.x)
	{
		if( pointB.y > pointA.y)
		{
			for(int j = yIntercept - (width/2); j < (yIntercept + (width/2)); j++)
			{
				//keep going as long as x and y values havent exceeded pointB
				for(int i = pointA.x; (temp_y < pointB.y) && (i < pointB.x) ;i++)	
				{
					temp_y = slope*i + j;
					putPixel(i, temp_y, default_lineColor, default_surface);
				}
			}
		}
		else if( pointB.y < pointA.y)
		{

			for(int j = yIntercept - (width/2); j < (yIntercept + (width /2)); j++)
			{
				//(temp_y+2 > pointB.y) &&
				//keep going as long as x and y values havent exceeded pointB
				for(int i = pointA.x;  (i < pointB.x) ;i++)	
				{
					temp_y = slope*i + j;
					putPixel(i, temp_y, default_lineColor, default_surface);
				}
			}
		}
		else if( pointB.y == pointA.y)
		{
			temp_y = pointB.y;

			for(int j = pointB.y - (width/2); j < (pointB.y + (width/2)); j++)
			{
				//keep going as long as x and y values havent exceeded pointB
				for(int i = pointA.x; (i < pointB.x) ;i++)	
				{
					putPixel(i, j, default_lineColor, default_surface);
				}
			}
		}
	}
	else if( pointB.x < pointA.x)
	{
		if( pointB.y > pointA.y)
		{
			for(int j = yIntercept - (width/2); j< (yIntercept + (width/2)); j++)
			{
				//keep going as long as x and y values havent exceeded pointB
				for(int i = pointA.x; (temp_y < pointB.y) || (i > pointB.x) ;i--)	
				{
					temp_y = slope*i + j;
					putPixel(i, temp_y, default_lineColor, default_surface);
				}
			}
		}
		else if( pointB.y < pointA.y)
		{
			for(int j = yIntercept - (width/2); j < (yIntercept + (width/2)); j++)
			{
				//keep going as long as x and y values havent exceeded pointB
				for(int i = pointA.x; (temp_y > pointB.y) || (i > pointB.x) ;i--)	
				{
					temp_y = slope*i + j;
					putPixel(i, temp_y, default_lineColor, default_surface);
				}
			}
		}
		else if( pointB.y == pointA.y)
		{
			temp_y = pointB.y;
			for(int j = pointB.y - (width/2); j < (pointB.y + (width/2)); j++)
			{
				//keep going as long as x and y values havent exceeded pointB
				for(int i = pointA.x; (i > pointB.x) ;i--)	
				{
					putPixel(i, j, default_lineColor, default_surface);
				}
			}
		}
	}
	else if( pointB.x == pointA.x)
	{
		if( pointB.y > pointA.y)
		{
			for(int j = pointB.x - (width/2); j < (pointB.x + (width/2)) ; j++)
			{
				//keep going as long as y values havent exceeded pointB
				for(int i = pointA.y; (i < pointB.y) ;i++)	
				{
					putPixel(j, i, default_lineColor, default_surface);
				}
			}
		}
		else if( pointB.y < pointA.y)
		{
			for(int j = pointB.x - (width/2); j < (pointB.x + (width/2)) ; j++)
			{
				//keep going as long as y values havent exceeded pointB
				for(int i = pointA.y; (i > pointB.y); i--)	
				{
					putPixel(j, i, default_lineColor, default_surface);
				}
			}
		}
	}
	//}}}

}

void Primitives::drawLine(CoOrd pointA, CoOrd pointB, int width, SDL_Surface *surface)
{
	//{{{
	float slope = calcSlope(pointA, pointB);
	float yIntercept = (float)(pointA.y)-(slope * (float) pointA.x);
	int temp_y = 0; 

	if( pointB.x > pointA.x)
	{
		if( pointB.y > pointA.y)
		{
			for(int j = yIntercept - (width/2); j < (yIntercept + (width/2)); j++)
			{
				//keep going as long as x and y values havent exceeded pointB
				for(int i = pointA.x; (temp_y < pointB.y) && (i < pointB.x) ;i++)	
				{
					temp_y = slope*i + j;
					putPixel(i, temp_y, default_lineColor, surface);
				}
			}
		}
		else if( pointB.y < pointA.y)
		{

			for(int j = yIntercept - (width/2); j < (yIntercept + (width /2)); j++)
			{
				//(temp_y+2 > pointB.y) &&
				//keep going as long as x and y values havent exceeded pointB
				for(int i = pointA.x;  (i < pointB.x) ;i++)	
				{
					temp_y = slope*i + j;
					putPixel(i, temp_y, default_lineColor, surface);
				}
			}
		}
		else if( pointB.y == pointA.y)
		{
			temp_y = pointB.y;

			for(int j = pointB.y - (width/2); j < (pointB.y + (width/2)); j++)
			{
				//keep going as long as x and y values havent exceeded pointB
				for(int i = pointA.x; (i < pointB.x) ;i++)	
				{
					putPixel(i, j, default_lineColor, surface);
				}
			}
		}
	}
	else if( pointB.x < pointA.x)
	{
		if( pointB.y > pointA.y)
		{
			for(int j = yIntercept - (width/2); j< (yIntercept + (width/2)); j++)
			{
				//keep going as long as x and y values havent exceeded pointB
				for(int i = pointA.x; (temp_y < pointB.y) || (i > pointB.x) ;i--)	
				{
					temp_y = slope*i + j;
					putPixel(i, temp_y, default_lineColor, surface);
				}
			}
		}
		else if( pointB.y < pointA.y)
		{
			for(int j = yIntercept - (width/2); j < (yIntercept + (width/2)); j++)
			{
				//keep going as long as x and y values havent exceeded pointB
				for(int i = pointA.x; (temp_y > pointB.y) || (i > pointB.x) ;i--)	
				{
					temp_y = slope*i + j;
					putPixel(i, temp_y, default_lineColor, surface);
				}
			}
		}
		else if( pointB.y == pointA.y)
		{
			temp_y = pointB.y;
			for(int j = pointB.y - (width/2); j < (pointB.y + (width/2)); j++)
			{
				//keep going as long as x and y values havent exceeded pointB
				for(int i = pointA.x; (i > pointB.x) ;i--)	
				{
					putPixel(i, j, default_lineColor, surface);
				}
			}
		}
	}
	else if( pointB.x == pointA.x)
	{
		if( pointB.y > pointA.y)
		{
			for(int j = pointB.x - (width/2); j < (pointB.x + (width/2)) ; j++)
			{
				//keep going as long as y values havent exceeded pointB
				for(int i = pointA.y; (i < pointB.y) ;i++)	
				{
					putPixel(j, i, default_lineColor, surface);
				}
			}
		}
		else if( pointB.y < pointA.y)
		{
			for(int j = pointB.x - (width/2); j < (pointB.x + (width/2)) ; j++)
			{
				//keep going as long as y values havent exceeded pointB
				for(int i = pointA.y; (i > pointB.y); i--)	
				{
					putPixel(j, i, default_lineColor, surface);
				}
			}
		}
	}
	//}}}
}

void Primitives :: putPixel(int x, int y, Uint32 color, SDL_Surface *surface)

{
//{{{

	if (SDL_MUSTLOCK(surface)) 
		SDL_LockSurface(surface);	

		Uint8 r, g, b, a; 
		
		// This will probably warn you about addressing locals 
		SDL_GetRGBA(color, surface->format, &r, &g, &b, &a); 
		 
		// Now give it transparent pixels 
		color = SDL_MapRGBA(surface->format, r, g, b, a); 

		int bpp = surface->format->BytesPerPixel;

		/* Here p is the address to the pixel we want to set */
		Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

		//prevent placeing pixel outside of screens bounds	
//		if( ((y*(surface->pitch/4))
//		    < game_height*game_width) 
//		    && (y*(surface->pitch/4)) >=0 )
				*(Uint32 *)p = color;

	if( SDL_MUSTLOCK(surface))
		SDL_UnlockSurface(surface);
//}}}
}

SDL_Surface* Primitives :: Get_Cached(int id)
{
	return surface_cache[id]->surface;
}
