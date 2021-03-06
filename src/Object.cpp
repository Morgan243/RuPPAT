//generic representation for interacting sprites (moving/alive/etc)

#include "Object.h"
#include "PhysFuncs.h"

Object :: Object(string sprite_path, 
            int start_x,
	       	int start_y,
	       	int start_mass)
{
//{{{

    sprite = new Sprite(sprite_path, 360, 0);
	refMax = refCounter = -1;

	//set x and y
	descriptor.x = (float)start_x;
	descriptor.y = (float)start_y;

	//set mass
	descriptor.mass = start_mass;

    //init force vectors
    descriptor.xForce = descriptor.yForce = 0.0;

	timeCreated = SDL_GetTicks();
//}}}	
}

Object :: Object(string sprite_path,
	       	int start_x, 
			int start_y,
	       	int start_mass,
			int num_rotations,
	       	int starting_angle):
        hitCircleSprite()
{
//{{{
    sprite = new Sprite(sprite_path, num_rotations, starting_angle);
	numRotations = num_rotations;

	refMax = refCounter = -1;

	//set x and y
	descriptor.x = (float)start_x;
	descriptor.y = (float)start_y;

	//set mass
	descriptor.mass = start_mass;
	
    //init force vectors
    descriptor.xForce = descriptor.yForce = 0.0;

	timeCreated = SDL_GetTicks();
//}}}
}

Object::Object(string sprite_path,
	       int start_x,
	       int start_y,
	       int start_mass,
	       int num_rotations,
	       int starting_angle,
	       float xVel,
	       float yVel,
	       string HC_path):
        hitCircleSprite(HC_path)
{
//{{{
    sprite = new Sprite(sprite_path, num_rotations, starting_angle);

	refMax = refCounter = -1;

	//set x and y
	descriptor.x = (float)start_x;
	descriptor.y = (float)start_y;

	//initial velocities
	descriptor.xVel=xVel;
	descriptor.yVel=yVel;

	descriptor.xAcc=0.0;
	descriptor.yAcc=0.0;

	//set mass
	descriptor.mass = start_mass;
    
    //init force vectors
    descriptor.xForce = descriptor.yForce = 0.0;

	numRotations = num_rotations;

	timeCreated = SDL_GetTicks();

	pixelSprite_cache = sprite->getPixelSprite();

	cout<<"pixel Sprite_cache in object is :"<< pixelSprite_cache.size()<<endl;
	isDestroying = false;
//}}}
}

void Object::setID(int id)
{
	descriptor.ID = id;
}

Object::Object(const Object &src)
	:sprite(src.sprite)
{
//{{{
	refMax = src.refMax;
	refCounter = src.refCounter;

	numRotations = src.numRotations;

	exhaustX = src.exhaustX;
	exhaustY = src.exhaustY;

	descriptor = src.descriptor;
	
	pixelSprite_cache = src.pixelSprite_cache;

	timeCreated = SDL_GetTicks();

	cout<<"Copy created at:"<<timeCreated<<endl;
//}}}
}

Object :: Object(Sprite* obj_sprite,
            int start_x,
	       	int start_y,
	       	int start_mass)
{
//{{{

    sprite = obj_sprite;
	refMax = refCounter = -1;

	//set x and y
	descriptor.x = (float)start_x;
	descriptor.y = (float)start_y;

	//set mass
	descriptor.mass = start_mass;

	timeCreated = SDL_GetTicks();
//}}}	
}

Object :: Object(Sprite* obj_sprite,
	       	int start_x, 
			int start_y,
	       	int start_mass,
			int num_rotations,
	       	int starting_angle):
        hitCircleSprite()
{
//{{{
    //sprite = new Sprite(sprite_path, num_rotations, starting_angle);
    sprite = obj_sprite;
	numRotations = num_rotations;

	refMax = refCounter = -1;
	//set x and y
	descriptor.x = (float)start_x;
	descriptor.y = (float)start_y;

	//set mass
	descriptor.mass = start_mass;
	
	timeCreated = SDL_GetTicks();
//}}}
}

Object::Object(Sprite *obj_sprite,
	       int start_x,
	       int start_y,
	       int start_mass,
	       int num_rotations,
	       int starting_angle,
	       float xVel,
	       float yVel,
	       string HC_path):
        hitCircleSprite(HC_path)
{
//{{{
    //sprite = new Sprite(sprite_path, num_rotations, starting_angle);
    sprite = obj_sprite;

	refMax = refCounter = -1;

	//set x and y
	descriptor.x = (float)start_x;
	descriptor.y = (float)start_y;

	//initial velocities
	descriptor.xVel=xVel;
	descriptor.yVel=yVel;

	descriptor.xAcc=0.0;
	descriptor.yAcc=0.0;

	//set mass
	descriptor.mass = start_mass;

	numRotations = num_rotations;

	timeCreated = SDL_GetTicks();

	pixelSprite_cache = sprite->getPixelSprite();

	cout<<"pixel Sprite_cache in object is :"<< pixelSprite_cache.size()<<endl;
	isDestroying = false;
//}}}
}

Object & Object::operator=(const Object &src)
{
	sprite = (src.sprite);

	//TODO: need to create new Object and return it	
}

Object :: ~Object()
{
}

//{{{

//->Descriptior Getters and Setters
//{{{
void Object::setDescriptor(const Entity_desc new_desc)
{
	descriptor = new_desc;
}

Entity_desc Object::getDescriptor() const
{
	return descriptor;
}

Entity_desc* Object::getDescriptor_ref()
{
	return &descriptor;
}
//}}}


//->Rotation Rate Getters Setters and Manipulatorts
//{{{
void Object :: setRotationRate(const float rotRate)
{
	sprite->setRotationRate(rotRate);
}

float Object :: getRotationRate()
{
	return sprite->getRotationRate();
}

void Object :: incrementRotationRate()
{
	sprite->incrementRotationRate();
}

void Object :: decrementRotationRate()
{
	sprite->decrementRotationRate();
}
//}}}


//->Angle Getters, Setters, and Manipulators
//{{{
float Object :: getAngle()
{
	return sprite->getAngle();
}

void Object :: setAngleIndex(const int angleIndex)
{
	sprite->setAngleIndex(angleIndex);
}

void Object :: incrementAngleIndex()
{
	sprite->incrementAngleIndex();
}

void Object :: decrementAngleIndex()
{
	sprite->decrementAngleIndex();
}
//}}}

//->Sprite Getter, Setter, Updater
//{{{
bool Object :: updateSprite()
{
	sprite->updateSprite();
	return false;
}

SDL_Surface* Object :: getSprite()
{
	return sprite->getSprite();
}

SDL_Surface *Object :: getSprite(const int angle)
{
	return sprite->getSprite(angle);
}

SDL_Texture* Object :: getSprite_text()
{
	return sprite->getSprite_text();
}

SDL_Texture *Object :: getSprite_text(const int angle)
{
	return sprite->getSprite_text(angle);
}
//}}}

//->Attribute Getters and Setters
//{{{
int Object :: getMass() const
{
	return descriptor.mass;
}

int Object :: getX() const
{
	return descriptor.x;
}

int Object :: getY() const
{
	return descriptor.y;
}

void Object :: setXY(const int x, const int y)
{
	descriptor.x = x;
	descriptor.y = y;
}

float Object :: getXvel() const
{
	return descriptor.xVel;
}

float Object :: getYvel() const
{
	return descriptor.yVel;
}

float Object :: getXacc() const
{
	return descriptor.xAcc;
}

float Object :: getYacc() const
{
	return descriptor.yAcc;
}
//}}}

//}}}

void Object :: accelForward() 
{
	setAccelVectors(true);
}

void Object :: accelBackward()
{
	setAccelVectors(false);
}

void Object :: setAccelVectors(const bool forward)
{
//{{{

    float heading = sprite->getAngle();
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
//}}}
}


void Object :: getExhaustVectors(float &xVel, float &yVel)
{
	xVel=exhaustX;
	yVel=exhaustY;
}

void Object :: GetVectors_FrontRelative(float &xVect,
                                            float &yVect, 	
                                            const float degreeOffset,
                                            const float mag)
{
///{{{
	//determine current angle of the front of sprite (zero degrees of base sprite)
	//determine new angle given the offset
	float angle = sprite->getAngle() + degreeOffset;

	if(angle>=360)
	{
		angle = angle - 360;
	}
	else if(angle<0)
	{
		angle = 360 + angle;
	}

	//use mag and angle to calc x and y vectors
	cout<<"ANGLE="<<angle<<endl;
	
	//convert to rads
	angle *= 0.0174532925;

	yVect = (-1)*sin(angle)*mag;
	xVect = cos(angle)*mag;
//}}}
}

Entity_desc* Object :: PhysicsHandler(const float t,
                                            const float dt, 
                                            Entity_desc &state_src)
{
//{{{
    //keep track of current time
	thisTime = t;

	//integrate the sprite location if its alive
	if(!isDestroying)
		PhysFunc::integrate(descriptor, t, dt, state_src);

	//handle pixels related/tied to object
	for(int i = 0; i < to_render.pixels.size(); i++)
	{
		//dim the pixel
		Common::ApplyDimming(to_render.pixels[i]);

		//if dimmed out, erase from vector
		if(to_render.pixels[i].deleteMe)
			to_render.pixels.erase(to_render.pixels.begin()+i);

		//bounce off game bounds
		Common::TestBounds(to_render.pixels[i], true);

		//integrate to find new position
		PhysFunc::integrate(to_render.pixels[i], t, dt, state_src);
	}

	//make sure this missile is removed if no longer used
	if(!to_render.pixels.size() && !to_render.sprites.size() && isDestroying)
		this->killMe = true;

	return &descriptor;
//}}}
}

Entity_desc* Object :: PhysicsHandler(Entity_desc &state_dest, 
											const float t, 
											const float dt)
{
//{{{
	thisTime = t;
	
	//default for of this constructor
	PhysFunc::integrate(state_dest, t, dt, descriptor);
	return &state_dest;
//}}}
}

Entity_desc* Object :: PhysicsHandler_force(const float t,
                                            const float dt, 
                                            Entity_desc &state_src)
{
//{{{
    //keep track of current time
	thisTime = t;

	//integrate the sprite location if its alive
	if(!isDestroying)
		PhysFunc::integrate(descriptor, t, dt, state_src);

	//handle pixels related/tied to object
	for(int i = 0; i < to_render.pixels.size(); i++)
	{
		//dim the pixel
		Common::ApplyDimming(to_render.pixels[i]);

		//if dimmed out, erase from vector
		if(to_render.pixels[i].deleteMe)
			to_render.pixels.erase(to_render.pixels.begin()+i);

		//bounce off game bounds
		Common::TestBounds(to_render.pixels[i], true);

		//integrate to find new position
		PhysFunc::integrate(to_render.pixels[i], t, dt, state_src);
	}

	//make sure this missile is removed if no longer used
	if(!to_render.pixels.size() && !to_render.sprites.size() && isDestroying)
		this->killMe = true;

	return &descriptor;
//}}}
}

Entity_desc* Object :: PhysicsHandler_force(Entity_desc &state_dest, 
											const float t, 
											const float dt)
{
//{{{
	thisTime = t;
	
	//default for of this constructor
	PhysFunc::integrate(state_dest, t, dt, descriptor);
	return &state_dest;
//}}}
}


float Object::setTimeNow(const float time)
{
//{{{
	//pass in -1 to just use ticks
	if(time >= 0)
		thisTime = time;
	else
		thisTime = SDL_GetTicks();

	//returns the time thats been set
	return thisTime;
//}}}
}

float Object::getTime()
{
	return thisTime;
}

void Object::GameDestroy()
{
//{{{
	//should append in case there are pixels already here
	to_render.pixels = pixelSprite_cache;
	
	int width, height;
	
	this->sprite->getDimensions(width,height);

    float theta = this->getAngle();

    CoOrd center;
        center.x = width/2;
        center.y = height/2;
 
	//set the correct coordinates
	for(int i = 0; i < to_render.pixels.size(); i++)
	{
		to_render.pixels[i].dimFactor = 18;
		to_render.pixels[i].dimTimer = 0;
        
        Common::RotatePoint(theta, to_render.pixels[i], center);

        to_render.pixels[i].x += (descriptor.x);
        to_render.pixels[i].y += (descriptor.y);

        to_render.pixels[i].xVel = descriptor.xVel
						+ rand()%75 - rand()%75;
        to_render.pixels[i].yVel = descriptor.yVel
						+ rand()%75 - rand()%75;
	}
	isDestroying = true;
//}}}
}

bool Object::GetRenderables(Renderables_Cont &renderables)
{
	renderables = this->to_render;
}

Renderables_Cont* Object::GetRenderables()
{
	return &to_render;
}

bool Object::UpdateAndGetRenderables(Renderables_Cont rnder)
{
}

//void Object::buildHitBoxes_fromLayer(SDL_Surface *HB_surface)
//{
////{{{
//	int spriteWidth, spriteHeight;
//	hitCircleSprite.getDimensions(spriteWidth,spriteHeight);
//
//	Uint32 spriteMape[spriteWidth][spriteHeight];
//
//	Uint32 temp_pixel;
//	Circle_desc temp_circle;
//	//scan the hitCircle sprite for a colored pixel	
//	for(int i=0; i<spriteWidth;i++)
//	{
//		for(int j=0; j<spriteHeight;j++)
//		{
//			if( (temp_pixel=hitCircleSprite.getPixel(i,j)))
//			{
//				temp_circle.x = i;
//				temp_circle.y = j;	
//				temp_circle.r = 0x00FF & (unsigned int)temp_pixel;
//		cout<<"HC found at ("<<i<<","<<j<<") r="<<temp_circle.r<<endl;
//			descriptor.hitCircles.push_back(temp_circle);
//			}
//		}
//	}
////}}}
//}
//
////return true if collision detected, sets velocity values too
//bool Object::checkHits(float &xVel, float &yVel, Entity_desc &check_ent)
//{
////{{{
//	int distance =( sqrt( ((descriptor.x-check_ent.x)*(descriptor.x-check_ent.x)) + ((descriptor.y-check_ent.y)*(descriptor.y-check_ent.y))  ) );
//	//if the entities are closer than the sum of the radiuses
//	if(descriptor.hitCircles.size() && check_ent.hitCircles.size())
//	{
//		if(distance< (descriptor.hitCircles[0].r + check_ent.hitCircles[0].r))
//		{
//		//collision!
//			
//
//			xVel *= (-1);
//			yVel *= (-1);
//
//			check_ent.x *= (-1);
//			check_ent.y *= (-1);
//	//std::cout<<"COLLISION!"<<std::endl;
//			return true;
//		}
//		else
//		{
//
//	//std::cout<<"nothin! "<<distance<<std::endl;
//			return false;
//		}
//	}
////}}}
//}

