//generic representation for interacting sprites (moving/alive/etc)

#include "Object.h"


Object :: Object(string sprite_path, int start_x, int start_y, int start_mass)
:sprite(sprite_path,360,0)
{//{{{

	//set x and y
	descriptor.x = (float)start_x;
	descriptor.y = (float)start_y;

	//set mass
	descriptor.mass = start_mass;
		
}//}}}

Object::Object(string sprite_path, int start_x, int start_y, int start_mass,
			int num_rotations, int starting_angle)
:sprite(sprite_path,num_rotations,starting_angle)
{
	//set x and y
	descriptor.x = (float)start_x;
	descriptor.y = (float)start_y;

	//set mass
	descriptor.mass = start_mass;
}

Object::Object(string sprite_path, int start_x, int start_y, int start_mass,
			int num_rotations, int starting_angle,
				 float xVel, float yVel)
:sprite(sprite_path,num_rotations,starting_angle)
{
	//set x and y
	descriptor.x = (float)start_x;
	descriptor.y = (float)start_y;

	descriptor.xVel=xVel;
	descriptor.yVel=yVel;
	//set mass
	descriptor.mass = start_mass;
}


Object :: ~Object()
{

}

void Object::setDescriptor(Entity_desc new_desc)
{
	descriptor = new_desc;
}

Entity_desc Object::getDescriptor()
{
	return descriptor;
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
	return sprite.getSprite();
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
