#include "Sprite.h"

Sprite::Sprite()
{

}

Sprite :: Sprite(string path_to_sprite)
{
	SDL_Surface *tempSprite, *tempSpriteOpt;
	
	tempSprite = IMG_Load((char *)path_to_sprite.c_str());

	tempSpriteOpt = SDL_DisplayFormatAlpha(tempSprite);

	sprite_surf = tempSpriteOpt;
}

Sprite::Sprite(string path_to_sprite,int numRotations, int startingAngle)
{
	currentAngleIndex = startingAngle;
	currentAngleIndex_f = startingAngle;

	rotationRate = 0.0;

	rotationalErrorAccum = 0.0;

	lastErr = 0.0;

	degreeIncrement = 360.0/numRotations;


	SDL_Surface *tempSprite, *tempSpriteOpt;

	tempSprite = IMG_Load((char *)path_to_sprite.c_str());

	tempSpriteOpt = SDL_DisplayFormatAlpha(tempSprite);

	sprite_surf = tempSpriteOpt;


	for(int i = 0; i<numRotations;i++)
	{
		rotations.push_back(rotozoomSurface(tempSpriteOpt,
							i*degreeIncrement,
							1.0,0));	
	}
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
	//apply current rotation rate
	currentAngleIndex_f += rotationRate;

	//prevent value from exceeding 360
	if(currentAngleIndex_f >= 360.0)currentAngleIndex_f = 0.0;

	//prevent value from dropping below 0
	if(currentAngleIndex_f < 0.0)currentAngleIndex_f = 359.0;
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

void Sprite::putPixel(int x, int y, Uint32 color, int screenID)
{
if (SDL_MUSTLOCK(sprite_surf)) 
	SDL_LockSurface(sprite_surf);	

//	unsigned int *ptr = (unsigned int*)mainScreen->pixels;
//	int lineoffset = y * (mainScreen->pitch / 4);
//	
//
//	int point = lineoffset + x;	
//
//	//prevent placeing pixel outside of screens bounds
//	if((point < mainHeight*mainWidth) && (point>=0))
//		ptr[point] = color;

Uint8 r, g, b, a; 
// This will probably warn you about addressing locals 
 SDL_GetRGBA(color, sprite_surf->format, &r, &g, &b, &a); 

 // Now give it transparent pixels 
 color = SDL_MapRGBA(sprite_surf->format, r, g, b, SDL_ALPHA_TRANSPARENT); 


    int bpp = sprite_surf->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)sprite_surf->pixels + y * sprite_surf->pitch + x * bpp;
	
	*(Uint32 *)p = color;

if( SDL_MUSTLOCK(sprite_surf) )
        SDL_UnlockSurface(sprite_surf);
}


void Sprite::getDimensions(int &w, int &h)
{
	w = sprite_surf->w;
	h = sprite_surf->h;
}
