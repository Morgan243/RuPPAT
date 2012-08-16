//generic representation for interacting sprites (moving/alive/etc)

#include "Object.h"


Object :: Object(string sprite_path, int start_x, int start_y, int start_mass)
{//{{{
	//load the sprite from sprite_path
	SDL_Surface *tempSprite, *tempSpriteOpt;

		tempSprite = IMG_Load((char *)sprite_path.c_str());
	
		tempSpriteOpt = SDL_DisplayFormatAlpha(tempSprite);

		sprite = tempSpriteOpt;

	//set x and y
	descriptor.x = (float)start_x;
	descriptor.y = (float)start_y;

	//set mass
	descriptor.mass = start_mass;
		
}//}}}

Object :: ~Object()
{

}


Mass_desc Object :: getDesc()
{
	Mass_desc mass;
	mass.mass = descriptor.mass;
	mass.xLoc = descriptor.x;
	mass.yLoc = descriptor.y;
	return mass; 
}



SDL_Surface * Object :: getSprite()
{
	return sprite;
}


int Object :: getX()
{
	return descriptor.x;
}

int Object :: getY()
{
	return descriptor.y;
}

float Object :: getXvel()
{
	return descriptor.xVel;
}

float Object :: getYvel()
{
	return descriptor.yVel;
}

float Object :: getXacc()
{
	return descriptor.xAcc;
}

float Object :: getYacc()
{
	return descriptor.yAcc;


}


//void Object :: accelForward()
//{
//
//
//}
//
//void Object :: accelBackward()
//{
//
//}
