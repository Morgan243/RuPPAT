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

	base_sprite = tempSpriteOpt;
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

	base_sprite = tempSpriteOpt;



	for(int i = 0; i<numRotations;i++)
	{
		rotations.push_back(
				rotozoomSurface(tempSpriteOpt,
							i*degreeIncrement,
							1.0,0));	
		SDL_SetAlpha(rotations.back(),0, 0x00);
		//cout<<"generating rotation #"<<i<<endl;
	}
	
	generateSpriteOutlines();

//}}}
}

Sprite::Sprite(const Sprite &src)
{
	SDL_Surface *tempSurf;
	src.copyAll(tempSurf, *this);
	//this->setBaseSprite(tempSurf);
	this->base_sprite = SDL_ConvertSurface(src.getBaseSprite(),
		       src.getBaseSprite()->format, SDL_SWSURFACE);
	this->generateRotations();

}

Sprite & Sprite::operator=(const Sprite &src)
{
	//copy the right hand side to the left hand side
	//src.copyBaseSprite(base_sprite);
	SDL_Surface *tempSurf = NULL;
	src.copyAll(tempSurf, *this);
	this->setBaseSprite(tempSurf);
	this->generateRotations();
}

Sprite::~Sprite()
{
//{{{
	for(int i = 0; i < rotations.size(); i++)
	{
		SDL_FreeSurface(rotations[i]);
	}
	SDL_FreeSurface(base_sprite);
//}}}
}
void Sprite :: copyAll(SDL_Surface *dest, Sprite &sprDest) const
{
//{{{
	float rotation_rate, degree_increment, current_angle;

	copyBaseSprite(dest);
	
	get_RotR_DegIncr_Angl(rotation_rate, degree_increment, current_angle);
	cout<<"In Cop Constr!::"<<endl;
	cout<<"rotRate: "<< rotation_rate<<endl;
	cout<<"DegIncr: "<< degree_increment<<endl;
	cout<<"CurrAngle: "<<current_angle<<endl;
	sprDest.set_RotR_DegIncr_Angl(rotation_rate, degree_increment, current_angle);

//}}}
}

void Sprite :: copyBaseSprite(SDL_Surface *dest) const
{
//{{{
	Uint32 rmask, gmask, bmask, amask;
	unsigned int *newPix = NULL, surfSize;
	int w, h, bpp;
	this->getDimensionsBpp(w,h,bpp);
	
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	    rmask = 0xff000000;
	    gmask = 0x00ff0000;
	    bmask = 0x0000ff00;
	    amask = 0x000000ff;
	#else
	    rmask = 0x000000ff;
	    gmask = 0x0000ff00;
	    bmask = 0x00ff0000;
	    amask = 0xff000000;
	#endif    

	
	if(SDL_MUSTLOCK(base_sprite))
		SDL_LockSurface(base_sprite);

		//copy pixel data from src to the temp (returnable) sprite
		dest = SDL_ConvertSurface(base_sprite, base_sprite->format, SDL_SWSURFACE);

	if(SDL_MUSTLOCK(base_sprite))
		SDL_UnlockSurface(base_sprite);

	//destination should be assigned at this point
	if(dest == NULL)
	{
		cout<<"Error creating surface in operator= of Sprite: "<<SDL_GetError()<<endl;
	}

//}}}
}

void Sprite :: get_RotR_DegIncr_Angl(float &rotateRate, float &degreeInc
						, float &currAngle) const
{
//{{{
	rotateRate = this->rotationRate;
	degreeInc = this->degreeIncrement;
	currAngle = this->currentAngleIndex_f;
//}}}
}

void Sprite :: set_RotR_DegIncr_Angl(float rotateRate, float degreeInc, float currAngle)
{
//{{{

	this->rotationRate = rotateRate;
	this->degreeIncrement = degreeInc;
	this->currentAngleIndex_f = currAngle;
	this->currentAngleIndex = (int)currAngle;
cout<<"IN SET, degree increment= "<<this->degreeIncrement<<endl;

//}}}
}

void Sprite::setBaseSprite()
{

}

void Sprite::setBaseSprite(string path_to_sprite)
{

}

void Sprite::setBaseSprite(SDL_Surface *src)
{
//{{{
	if(base_sprite != NULL)
	{
		SDL_FreeSurface(base_sprite);
	}

	base_sprite = src;
//}}}
}

void Sprite :: generateRotations()
{
//{{{
	rotationalErrorAccum = 0.0;

	lastErr = 0.0;

	int numRotations = 360.0/degreeIncrement;

	SDL_Surface *tempSprite, *tempSpriteOpt;
//
//	tempSprite = IMG_Load((char *)sprite_path.c_str());
//
//	tempSpriteOpt = SDL_DisplayFormatAlpha(tempSprite);
//
//	base_sprite = tempSpriteOpt;
//
	tempSpriteOpt = SDL_DisplayFormatAlpha(base_sprite);

	cout<<"Generate Rots = "<<numRotations<<endl;

	for(int i = 0; i<numRotations;i++)
	{
		//cout<<"i="<<i<<" degIncr="<<degreeIncrement<<endl;
		rotations.push_back(
				rotozoomSurface(tempSpriteOpt,
							i*degreeIncrement,
							1.0,0));	
		SDL_SetAlpha(rotations.back(), 0, 0xFF);
	}
	
	generateSpriteOutlines();
//}}}
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


SDL_Surface* Sprite::getBaseSprite() const
{
	return base_sprite;
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
	Uint32 *pixels = (Uint32 *)base_sprite->pixels;
	return pixels[ (y * base_sprite->w) + x];
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
	w = base_sprite->w;
	h = base_sprite->h;
}

void Sprite::getDimensionsBpp(int &w, int &h, int &bpp) const
{
	w = base_sprite->w;
	h = base_sprite->h;
	bpp = base_sprite->format->BitsPerPixel;
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
//{{{

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
//}}}
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
