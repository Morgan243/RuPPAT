#include "math.h"
#include "Sprite.h"
#include <iostream>
#include <iomanip>

Sprite::Sprite()
{

}

Sprite :: Sprite(string path_to_sprite)
{
//{{{
	SDL_Surface *tempSprite, *tempSpriteOpt;
	
	tempSprite = IMG_Load((char *)path_to_sprite.c_str());

	tempSpriteOpt = SDL_DisplayFormatAlpha(tempSprite);

	sprite_surf = tempSpriteOpt;
//}}}
}

Sprite::Sprite(string path_to_sprite,int numRotations, int startingAngle)
{
//{{{
	currentAngleIndex = startingAngle;
	currentAngleIndex_f = startingAngle;

	rotationRate = 0.0;

	rotationalErrorAccum = 0.0;

	lastErr = 0.0;

	degreeIncrement = 360.0/numRotations;


	SDL_Surface *tempSprite, *tempSpriteOpt;

	sprite_path = path_to_sprite;

	tempSprite = IMG_Load((char *)path_to_sprite.c_str());

	tempSpriteOpt = SDL_DisplayFormatAlpha(tempSprite);

	sprite_surf = tempSpriteOpt;



	for(int i = 0; i<numRotations;i++)
	{
		rotations.push_back(
				rotozoomSurface(tempSpriteOpt,
							i*degreeIncrement,
							1.0,0));	
		SDL_SetAlpha(rotations.back(), 0, 0xFF);
	}
	
	generateSpriteOutlines();

//	clearPixelFromAll(0,1,1);
//	clearPixelFromAll(0,2,1);
//	clearPixelFromAll(0,1,2);
//	clearPixelFromAll(0,2,2);
//	clearPixelFromAll(0,3,3);
//
//	clearPixelFromAll(0,5,5);
//	clearPixelFromAll(0,6,5);
//	clearPixelFromAll(0,5,6);
//	clearPixelFromAll(0,6,6);
//	clearPixelFromAll(0,7,7);
//}}}
}

Sprite::~Sprite()
{

}


void Sprite::setRotationRate(float rotRate)
{
	rotationRate = rotRate;
}


float Sprite::getRotationRate()
{
	return rotationRate;
}

void Sprite::incrementRotationRate()
{
	rotationRate += 0.01;
}


void Sprite::decrementRotationRate()
{
	rotationRate -= 0.01;
}


void Sprite::updateSprite()
{
//{{{
	//apply current rotation rate
	currentAngleIndex_f += rotationRate;

	//prevent value from exceeding 360
	if(currentAngleIndex_f >= 360.0)currentAngleIndex_f = 0.0;

	//prevent value from dropping below 0
	if(currentAngleIndex_f < 0.0)currentAngleIndex_f = 359.0;
//}}}
}


float Sprite::getAngle()
{
	return (float)currentAngleIndex_f*degreeIncrement;
}


void Sprite::setAngleIndex(int angleIndex)
{
	currentAngleIndex = angleIndex;
}


void Sprite::incrementAngleIndex()
{
	currentAngleIndex++;
}


void Sprite::decrementAngleIndex()
{
	currentAngleIndex--;
}


SDL_Surface* Sprite::getBaseSprite()
{
	return sprite_surf;
}


SDL_Surface* Sprite::getSprite()
{
	int tmp_angle = (int) currentAngleIndex_f;
	return rotations[tmp_angle]; 
}


SDL_Surface* Sprite::getSprite(int angle)
{
	return rotations[angle];
}


Uint32 Sprite::getPixel(int x, int y)
{
	Uint32 *pixels = (Uint32 *)sprite_surf->pixels;
	return pixels[ (y * sprite_surf->w) + x];
}
Uint32 Sprite::getPixel(int x, int y, int rotation_i)
{
	Uint32 *pixels = (Uint32 *)rotations[rotation_i]->pixels;
	return pixels[ (y * rotations[rotation_i]->w) + x];
}

void Sprite::putPixel(int x, int y, Uint32 color, int rotation_i)
{
//{{{
if (SDL_MUSTLOCK(rotations[rotation_i])) 
	SDL_LockSurface(rotations[rotation_i]);	

	Uint8 r, g, b, a; 

	// This will probably warn you about addressing locals 
	SDL_GetRGBA(color, rotations[rotation_i]->format, &r, &g, &b, &a); 

	// Now give it transparent pixels 
	color = SDL_MapRGBA(rotations[rotation_i]->format, r, g, b, SDL_ALPHA_TRANSPARENT); 

	int bpp = rotations[rotation_i]->format->BytesPerPixel;
	
	// Here p is the address to the pixel we want to set 
	Uint8 *p = 
	(Uint8 *)rotations[rotation_i]->pixels + y * rotations[rotation_i]->pitch + x * bpp;
	
	*(Uint32 *)p = color;


if (SDL_MUSTLOCK(rotations[rotation_i])) 
        SDL_UnlockSurface(rotations[rotation_i]);

//}}}
}


void Sprite::getDimensions(int &w, int &h)
{
	w = sprite_surf->w;
	h = sprite_surf->h;
}


void Sprite::generateSpriteOutlines()
{
//{{{	

	for(int k = 0; k < rotations.size(); k++)
	{
		unsigned int pixVal[rotations[k]->w][rotations[k]->h]; 

		for(int i = 0; i < rotations[k]->h; i++)
		{
			for(int j = 0; j < rotations[k]->w; j++)
			{

				if(((pixVal[j][i] = getPixel(j,i)) != 0) 
						&& (k == 0))
					printf("1 ");
				else if (k==0)
					printf("_ ");
			}
			if(!k)
			printf("\n");
			//pixMaps.push_back(&pixVal);
		}
	}
///}}}
}


vector<CoOrd> Sprite:: outlineSprite()
{
//{{{
//	for(int i = 0; i < sprite->w; i++)
	{
		//for(int j = 0; j < sprite->h; j++)
		{

		}
	}
//}}}
}

void Sprite::clearPixelFromAll( int start_rot_i, int x, int y)
{

	int tempX = x, tempY = y, index=start_rot_i;
	float theta, mag;
	
	cout<<"input x,y: "<<x<<" , "<<y<<endl;

	mag = sqrt((float)(tempX*tempX) + (float)(tempY*tempY));
	cout<<"mag = "<<mag<<endl;	
	//invert Y
	tempY *= (-1.0);
	//add width and height
	tempY += (rotations[index]->h/2);
	tempX += (rotations[index]->w/2);
	
	theta = atan( ((float)tempY) / ((float)tempX) );
	
	//x and y both are not zero!
	if(x && y)
	{
		//find initial theta
		if(y>0)//first or second quadrant
		{
			if(x>0)//first quad
			{
				//do nothing
			}
			else//second quad
			{
				theta = theta*(-1.0) + 90.0;
			}

		}
		else//third or fourth quad
		{
			if(x>0)
			{
				theta = theta*(-1.0) + 270.0;
			}
			else
			{
				theta = theta + 180.0;
			}
		}
	}


	cout<<"SPRITE: "<<sprite_path<<endl;
	//go through all roations
	//i is NOT the index, must start from arg
	for(int i=0;i<rotations.size();i++)
	{
		cout<<"("<<index<<")CLEARING: "<<tempX<<" , "<<tempY<<endl;
		cout<<"\tTheat = "<<theta<<endl;
		clearPixel(index, tempX, tempY);	
		theta += (degreeIncrement*0.0174532925);
		
		tempY = sin(theta)*mag;
		tempX = cos(theta)*mag;

		//invert Y
		tempY *= (-1.0);

		//add width and height
		tempY += (rotations[index]->h/2);
		tempX += (rotations[index]->w/2);
		
		if(index != 359)
			index++;
		else
			index = 0;
	}

}


void Sprite::clearPixel(int rotation, int x, int y)
{
	//rotations[rotation]
	Uint32 tempColor = getPixel(x, y, rotation);
//	cout<<"PIXEL TEST: 0x"<<hex<<tempColor<<endl;	

	//clear alpha portion of the pixel to make it transparent
	tempColor = tempColor & 0xFFFFFF00;

	//cout<<"\t New PIXEL: 0x"<<hex<<tempColor<<endl;	

	putPixel(x,y,tempColor,rotation);
}
