#include "Object.h"

Object :: Object(string sprite_path, int start_x, int start_y, int start_mass)
{
	SDL_Surface *tempSprite, *tempSpriteOpt;

		tempSprite = IMG_Load((char *)sprite_path.c_str());
	
		tempSpriteOpt = SDL_DisplayFormatAlpha(tempSprite);

		sprite = tempSpriteOpt;

	descriptor.xLoc = (float)start_x;
	descriptor.yLoc = (float)start_y;

	descriptor.mass = start_mass;
		
}

Object :: ~Object()
{

}


Mass_desc Object :: getDesc()
{
	return descriptor;
}


SDL_Surface * Object :: getSprite()
{
	return sprite;
}


int Object :: getX()
{
	return descriptor.xLoc;
}

int Object :: getY()
{
	return descriptor.yLoc;
}
