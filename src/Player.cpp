//represents a player  

#include "Player.h"

//secondary-CONSTRUCTOR: leave many attributes statically defined by literals,
//good for any quick tests
Player :: Player(string sprite_path, int num_rotations, int startingAngle) 
	: Object(sprite_path, 200,200,1000), sprite(sprite_path, num_rotations, startingAngle)
//{{{
{
	currentAngle_index = startingAngle;	

	currentAngle_index_f = startingAngle;	

	rotationRate = 0.0;

	rotationalErrorAccum = 0.0;

	lastErr = 0.0;
	
	DEGREE_INCREMENT = 360.0/num_rotations;
	
	SDL_Surface *tempSprite, *tempSpriteOpt;

  //create all the rotated version of sprite (rotate loaded, save, repeat)
  for(int i = 0; i< num_rotations;i++)
	{
		tempSprite = IMG_Load((char *)sprite_path.c_str());
	
		tempSpriteOpt = SDL_DisplayFormat(tempSprite);

		rotations.push_back(rotozoomSurface(tempSpriteOpt, 
							i*DEGREE_INCREMENT,
							 1.0, 1));
	}

	descriptor.x = 200;
	descriptor.y = 200;
	descriptor.xVel = 0;
	descriptor.yVel = 0;
	descriptor.xAcc = 0;
	descriptor.yAcc = 0;
	descriptor.xGacc = 0;
	descriptor.yGacc = 0;
	descriptor.maxAccel = 1;
	descriptor.mass=1000;
	descriptor.noDrawEffect = false;

}
//}}}

//Constructor with more options
Player :: Player(string sprite_path, int num_rotations, int startingAngle,
		float maxaccel, int startX, int startY) 
		: Object(sprite_path,startX,startY,1000), sprite(sprite_path, 
								num_rotations,startingAngle)
//{{{
{
	currentAngle_index = startingAngle;	

	currentAngle_index_f = startingAngle;	

	rotationRate = 0.0;

	rotationalErrorAccum = 0.0;

	lastErr = 0.0;

	DEGREE_INCREMENT = 360.0/num_rotations;
	
	SDL_Surface *tempSprite, *tempSpriteOpt;

  for(int i = 0; i< num_rotations;i++)
	{
		tempSprite = IMG_Load((char *)sprite_path.c_str());
	
		tempSpriteOpt = SDL_DisplayFormatAlpha(tempSprite);

		rotations.push_back(rotozoomSurface(tempSpriteOpt, 
							i*DEGREE_INCREMENT,
							 1.0, 0));
	}

	descriptor.x = startX;
	descriptor.y = startY;
	descriptor.xVel = 0;
	descriptor.yVel = 0;
	descriptor.xAcc = 0;
	descriptor.yAcc = 0;
	descriptor.xGacc = 0;
	descriptor.yGacc = 0;
	descriptor.maxAccel = maxaccel;
	descriptor.noDrawEffect = false;

}
//}}}

//Deconstructor 
Player :: ~Player()
{


}

void Player :: setRotation_rate(float rotRate)
{
	sprite.setRotationRate(rotRate);
}

float Player :: getRotation_rate()
{
	return sprite.getRotationRate();
}

void Player :: incrementRotation_rate()
{
	sprite.incrementRotationRate();
}

void Player :: decrementRotation_rate()
{
	sprite.decrementRotationRate();

}

void Player :: updateSprite()
{
	sprite.updateSprite();
}

float Player :: getAngle()
{
	return sprite.getAngle();
}

void Player :: setAngle_index(int angleIndex)
{
	sprite.setAngleIndex(angleIndex);
}


void Player :: incrementAngle_index()
{
	sprite.incrementAngleIndex();
}


void Player :: decrementAngle_index()
{
	sprite.decrementAngleIndex();
}


SDL_Surface *Player :: getSprite()
{
	return sprite.getSprite();
}


SDL_Surface *Player :: getSprite(int angle)
{
	return sprite.getSprite(angle);
}

void Player :: setDescriptor(Entity_desc new_desc)
{
	descriptor = new_desc;
}

Entity_desc Player :: getDescriptor()
{
	return descriptor;
}


void Player :: accelForward() 
{
	setAccelVectors(true);
}

void Player :: accelBackward()
{
	setAccelVectors(false);
}

void Player :: setAccelVectors(bool forward)
{//{{{

		float heading = sprite.getAngle();
	if(forward)
	{

	
		if(heading>0 && heading<90)
			{
			exhaustX=-1*descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=(descriptor.maxAccel*sin(heading*3.141/180));


			descriptor.xVel += descriptor.maxAccel*cos(heading*3.141/180);
			descriptor.yVel +=-1*(descriptor.maxAccel*sin(heading*3.141/180));
			}
		else if(heading>90 && heading<180)
			{
			exhaustX=-1*descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=(descriptor.maxAccel*sin(heading*3.141/180));

			descriptor.xVel += descriptor.maxAccel*cos(heading*3.141/180);
			descriptor.yVel += -1*(descriptor.maxAccel*sin(heading*3.141/180));
			}
		else if(heading>180 && heading<260)
			{
			exhaustX=-1*descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=(descriptor.maxAccel*sin(heading*3.141/180));

			descriptor.xVel += descriptor.maxAccel*cos(heading*3.141/180);
			descriptor.yVel += -1*(descriptor.maxAccel*sin(heading*3.141/180));
			}
		else if(heading>260 && heading<360)
			{
			heading =360- heading;
			exhaustX=-1*descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=-1*(descriptor.maxAccel*sin(heading*3.141/180));

			descriptor.xVel += (descriptor.maxAccel*cos(heading*3.141/180));
			descriptor.yVel += (descriptor.maxAccel*sin(heading*3.141/180));
			}

		else if(heading == 0)
			{
			descriptor.xVel += descriptor.maxAccel;
			}


	}
	else
	{
	
		if(heading>0 && heading<90)
			{
			exhaustX=descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=-1*(descriptor.maxAccel*sin(heading*3.141/180));

			descriptor.xVel -= descriptor.maxAccel*cos(heading*3.141/180);
			descriptor.yVel -=-1*(descriptor.maxAccel*sin(heading*3.141/180));
			}
		else if(heading>90 && heading<180)
			{
			exhaustX=descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=-1*(descriptor.maxAccel*sin(heading*3.141/180));

			descriptor.xVel -= descriptor.maxAccel*cos(heading*3.141/180);
			descriptor.yVel -= -1*(descriptor.maxAccel*sin(heading*3.141/180));
			}
		else if(heading>180 && heading<260)
			{
			exhaustX=descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=-1*(descriptor.maxAccel*sin(heading*3.141/180));

			descriptor.xVel -= descriptor.maxAccel*cos(heading*3.141/180);
			descriptor.yVel -= -1*(descriptor.maxAccel*sin(heading*3.141/180));
			}
		else if(heading>260 && heading<360)
			{
			heading =360- heading;
			exhaustX=descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=(descriptor.maxAccel*sin(heading*3.141/180));

			descriptor.xVel -= (descriptor.maxAccel*cos(heading*3.141/180));
			descriptor.yVel -= (descriptor.maxAccel*sin(heading*3.141/180));
			}

		else if(heading == 0)
			{
			descriptor.xVel -= descriptor.maxAccel;
			}

	}
}
//}}}

void Player :: getXY_exhaust(float &xVel, float &yVel)
{
	xVel=exhaustX;
	yVel=exhaustY;
}



