//represents a player  

#include "Player.h"

//secondary-CONSTRUCTOR: leave many attributes statically defined by literals,
//good for any quick tests
Player :: Player(string sprite_path, int num_rotations, int startingAngle) 
	: Object(sprite_path, 200,200,1000)
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

//Constructor with more options
Player :: Player(string sprite_path, int num_rotations, int startingAngle,
		float maxaccel, int startX, int startY) 
		: Object(sprite_path,startX,startY,1000)
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

//Deconstructor 
Player :: ~Player()
{


}

void Player :: setRotation_rate(int rotRate)
{
	rotationRate = rotRate;

}

float Player :: getRotation_rate()
{
	return rotationRate;
}

void Player :: incrementRotation_rate()
{
	rotationRate += 0.01;
}

void Player :: decrementRotation_rate()
{
	rotationRate -= 0.01;

}

void Player :: updateSprite()
{
	currentAngle_index_f += rotationRate;
	if(currentAngle_index_f >=360.0)currentAngle_index_f=0.0;
	if(currentAngle_index_f < 0.0) currentAngle_index_f = 359.0;

}

float Player :: getAngle()
{
	return (float)currentAngle_index_f*DEGREE_INCREMENT;
}

void Player :: setAngle_index(int angleIndex)
{
	currentAngle_index = angleIndex;
}


void Player :: incrementAngle_index()
{
	currentAngle_index++;
}


void Player :: decrementAngle_index()
{
	currentAngle_index--;
}


SDL_Surface *Player :: getSprite()
{
	int tmp_angle = (int) currentAngle_index_f;
	return rotations[tmp_angle];
}


SDL_Surface *Player :: getSprite(int angle)
{
	return rotations[angle];
}

void Player :: setDescriptor(Entity_desc new_desc)
{
	descriptor = new_desc;
}

Entity_desc Player :: getDescriptor()
{
	return descriptor;
}

int Player :: getX()
{
	return descriptor.x;
}

int Player :: getY()
{
	return descriptor.y;
}


float Player :: getXvel()
{
	return descriptor.xVel;
}

float Player :: getYvel() 
{
	return descriptor.yVel;
}

float Player :: getXacc() 
{
	return descriptor.xAcc;
}

float Player :: getYacc() 
{
	return descriptor.yAcc;
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
{

		float heading = DEGREE_INCREMENT * currentAngle_index_f;
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

	//	heading = heading - 90;
			descriptor.xVel += descriptor.maxAccel*cos(heading*3.141/180);
			descriptor.yVel += -1*(descriptor.maxAccel*sin(heading*3.141/180));
			}
		else if(heading>180 && heading<260)
			{
			exhaustX=-1*descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=(descriptor.maxAccel*sin(heading*3.141/180));
		//	heading = heading -180;
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
			//descriptor.xAcc = descriptor.maxAccel*1000;
			descriptor.xVel += descriptor.maxAccel;
			//descriptor.yVel += 0;
			cout<<"Acceled at ZERO! "<<descriptor.xAcc<<endl;
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
		//	heading = heading - 90;
			exhaustX=descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=-1*(descriptor.maxAccel*sin(heading*3.141/180));

			descriptor.xVel -= descriptor.maxAccel*cos(heading*3.141/180);
			descriptor.yVel -= -1*(descriptor.maxAccel*sin(heading*3.141/180));
			}
		else if(heading>180 && heading<260)
			{
		//	heading = heading -180;
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
			//descriptor.xAcc = descriptor.maxAccel*1000;
			descriptor.xVel -= descriptor.maxAccel;
			//descriptor.yVel += 0;
			cout<<"Acceled at ZERO! "<<descriptor.xAcc<<endl;
			}

	}
}


void Player :: getXY_exhaust(float &xVel, float &yVel)
{
//	float heading = DEGREE_INCREMENT * currentAngle_index_f;
xVel=exhaustX;
yVel=exhaustY;
}



