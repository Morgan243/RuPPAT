//generic representation for interacting sprites (moving/alive/etc)

#include "Object.h"
#include "PhysFuncs.h"

int Object::next_obj_id = 0;

Object :: Object(string sprite_path, 
            int start_x,
	       	int start_y,
	       	int start_mass)
{
//{{{
    owning_object = NULL;

    sprite = new Sprite(sprite_path, 360, 0);
	refMax = refCounter = -1;

    this->descriptor.hitCircle_radius = sprite->getLargestDimension();

	//set x and y
	descriptor.x = (float)start_x;
	descriptor.y = (float)start_y;
    
    descriptor.xVel = descriptor.yVel = 0;

	//set mass
	descriptor.mass = start_mass;

    //init force vectors
    descriptor.xForce = descriptor.yForce = 0.0;

	timeCreated = SDL_GetTicks();

    descriptor.ID = next_obj_id++;

    this->sprite_silhouette = sprite->sprite_silhouette;

    this->descriptor.hit_slices = this->generateCollisionSlices(4);
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
    owning_object = NULL;

    sprite = new Sprite(sprite_path, num_rotations, starting_angle);

    this->descriptor.hitCircle_radius = sprite->getLargestDimension();

	numRotations = num_rotations;

	refMax = refCounter = -1;

	//set x and y
	descriptor.x = (float)start_x;
	descriptor.y = (float)start_y;

	//set mass
	descriptor.mass = start_mass;
	
    //init force vectors
    descriptor.xForce = descriptor.yForce = 0.0;

    descriptor.xVel = descriptor.yVel = 0;

	timeCreated = SDL_GetTicks();

    descriptor.ID = next_obj_id++;

    this->sprite_silhouette = sprite->sprite_silhouette;

    this->descriptor.hit_slices = this->generateCollisionSlices(4);
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
    owning_object = NULL;

    sprite = new Sprite(sprite_path, num_rotations, starting_angle);

    this->descriptor.hitCircle_radius = sprite->getLargestDimension();

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

    this->sprite_silhouette = sprite->sprite_silhouette;

    this->descriptor.hit_slices = this->generateCollisionSlices(4);

	cout<<"pixel Sprite_cache in object is :"<< pixelSprite_cache.size()<<endl;
	isDestroying = false;

    descriptor.ID = next_obj_id++;
    cout<<"Object ID: "<< descriptor.ID <<endl;
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

void Object :: setForceVectors(const bool forward)
{
//{{{

    float heading = sprite->getAngle();
	if(forward)
	{
		if(heading>0 && heading<90)
        {
			exhaustX=-1*descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=(descriptor.maxAccel*sin(heading*3.141/180));


			descriptor.xForce += descriptor.maxAccel*cos(heading*3.141/180);
			descriptor.yForce +=-1*(descriptor.maxAccel*sin(heading*3.141/180));
        }
		else if(heading>90 && heading<180)
        {
			exhaustX=-1*descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=(descriptor.maxAccel*sin(heading*3.141/180));

			descriptor.xForce += descriptor.maxAccel*cos(heading*3.141/180);
			descriptor.yForce += -1*(descriptor.maxAccel*sin(heading*3.141/180));
        }
		else if(heading>180 && heading<260)
        {
			exhaustX=-1*descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=(descriptor.maxAccel*sin(heading*3.141/180));

			descriptor.xForce += descriptor.maxAccel*cos(heading*3.141/180);
			descriptor.yForce += -1*(descriptor.maxAccel*sin(heading*3.141/180));
        }
		else if(heading>260 && heading<360)
        {
			heading =360- heading;
			exhaustX=-1*descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=-1*(descriptor.maxAccel*sin(heading*3.141/180));

			descriptor.xForce += (descriptor.maxAccel*cos(heading*3.141/180));
			descriptor.yForce += (descriptor.maxAccel*sin(heading*3.141/180));
        }

		else if(heading == 0)
        {
            exhaustX = -1*descriptor.maxAccel;
            exhaustY = 0;
            descriptor.xForce += descriptor.maxAccel;
        }
	}
	else if (!forward)
	{
		if(heading>0 && heading<90)
        {
			exhaustX=descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=-1*(descriptor.maxAccel*sin(heading*3.141/180));

			descriptor.xForce -= descriptor.maxAccel*cos(heading*3.141/180);
			descriptor.yForce -=-1*(descriptor.maxAccel*sin(heading*3.141/180));
        }
		else if(heading>90 && heading<180)
        {
			exhaustX=descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=-1*(descriptor.maxAccel*sin(heading*3.141/180));

			descriptor.xForce -= descriptor.maxAccel*cos(heading*3.141/180);
			descriptor.yForce -= -1*(descriptor.maxAccel*sin(heading*3.141/180));
        }
		else if(heading>180 && heading<260)
        {
			exhaustX=descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=-1*(descriptor.maxAccel*sin(heading*3.141/180));

			descriptor.xForce -= descriptor.maxAccel*cos(heading*3.141/180);
			descriptor.yForce -= -1*(descriptor.maxAccel*sin(heading*3.141/180));
        }
		else if(heading>260 && heading<360)
        {
			heading =360- heading;
			exhaustX=descriptor.maxAccel*cos(heading*3.141/180);
			exhaustY=(descriptor.maxAccel*sin(heading*3.141/180));

			descriptor.xForce -= (descriptor.maxAccel*cos(heading*3.141/180));
			descriptor.yForce -= (descriptor.maxAccel*sin(heading*3.141/180));
        }

		else if(heading == 0)
        {
            exhaustX = -1*descriptor.maxAccel;
            exhaustY = 0;
			descriptor.xForce -= descriptor.maxAccel;
        }
	}
//}}}
}

void Object :: getExhaustVectors(float &xVel, float &yVel)
{
	xVel = exhaustX;
	yVel = exhaustY;
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

Entity_desc* Object::updatePositional(float t, float dt)
{
    PhysFunc::integrate_force(this->descriptor, t, dt);
}

//override the phys check of this missile against other objects
Entity_desc* Object :: stateUpdate(const float t,
                                            const float dt, 
                                            Entity_desc &state_src)
{
//{{{
	//integrate the sprite location if its alive
	if(!isDestroying)
		PhysFunc::G_force(this->descriptor, state_src);

    //is it colliding with anything?
    //if(Common::isCircleIntersecting(this->descriptor, state_src))
    if(Common::areSliceseIntersecting(this->descriptor, state_src))
        //can't intersect owning object and can't already be in death seq
        if(this->owning_object != NULL && !this->isDestroying 
                && this->owning_object->descriptor.ID != state_src.ID)
        {
            //cout<<"x vel = "<<this->descriptor.xVel<<", mass = "<<descriptor.mass<<endl;
            state_src.xForce += 500*this->descriptor.xVel *this->descriptor.mass;
            state_src.yForce += 500*this->descriptor.yVel *this->descriptor.mass;
            this->GameDestroy();
        }

    if( last_update_t != t)
    {
        //keep track of current time
        last_update_t = t;

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
    }

	//make sure this missile is removed if no longer used
	if(!to_render.pixels.size() && !to_render.sprites.size() && isDestroying)
		this->killMe = true;

	return &descriptor;
//}}}
}

Entity_desc* Object :: stateUpdate(Entity_desc &state_dest, 
											const float t, 
											const float dt)
{
//{{{
	//thisTime = t;
	
	//default for of this constructor
	PhysFunc::G_force(state_dest, descriptor);
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
		to_render.pixels[i].dimFactor = 3;
		to_render.pixels[i].dimTimer = 0;
        
        Common::RotatePoint(theta, to_render.pixels[i], center);

        to_render.pixels[i].x += (descriptor.x);
        to_render.pixels[i].y += (descriptor.y);

        to_render.pixels[i].xVel = descriptor.xVel*4.0
						+ rand()%100 - rand()%100;
        to_render.pixels[i].yVel = descriptor.yVel*4.0
						+ rand()%100 - rand()%100;
	}

    //let everything know this is in its death sequence
	this->isDestroying = true;
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

CollisionCircleSlices_Container Object::generateCollisionSlices(int numSlices)
{
    //{{{
    float theta_granularity = 15.0;
    CollisionCircleSlices_Container slices_cont;
        slices_cont.num_slices = numSlices;
        slices_cont.deg_per_slice = 360.0/(float)numSlices;

    int num_checks_per_slice = slices_cont.deg_per_slice / theta_granularity;

    cout<<endl<<endl<<"Generating "<<numSlices<<" Slices!!"<<endl;
    int height = this->sprite_silhouette.size();
    int width = this->sprite_silhouette.at(0).size();

    int radial_height = height/2.0;
    int radial_width = width/2.0;

    cout<<"Silhouette height: "<<height<<endl;
    cout<<"Silhouette length: "<<width<<endl;
    cout<<"Silhouette radial height: "<<radial_height<<endl;
    cout<<"Silhouette radial length: "<<radial_width<<endl;

  int x, y;
  float radius = 0.0, max_radius = 0.0, last_radius, radius_sum = 0.0,
        slope = 0.0,
        theta = 0.0,
        slice_low_bound = 0.0,
        slice_high_bound = slices_cont.deg_per_slice;


  for(int i = 0; i < numSlices; i++)
  {
      slice_low_bound = i * slices_cont.deg_per_slice;
      slice_high_bound = (i+1) * slices_cont.deg_per_slice;

      bool edge_found;

        //get theta's for slice
        for(theta = slice_low_bound; theta < slice_high_bound; theta += theta_granularity)
        {
        //{{{            
            cout<<"Theta = "<< theta<<endl;
            //{{{
            if(theta <= 45.0 || theta > 315)
            {
                if(theta == 0)
                {
                    max_radius = radial_width;
                    cout<<"Max Radius: "<<max_radius<<endl;
                }
                else
                {
                    max_radius = Common::abs_val((float)radial_width/cos(theta*PI/180.0));
                    cout<<"Max Radius: "<<max_radius<<endl;
                }
            } 
            else if(theta <= 135.0)
            {
                if(theta == 90)
                {
                    max_radius = radial_height;
                    cout<<"Max Radius: "<<max_radius<<endl;
                }
                else
                {
                    max_radius = Common::abs_val((float)radial_height/sin(theta*PI/180.0));
                    cout<<"Max Radius: "<<max_radius<<endl;
                }
            }
            else if( theta <= 225.0)
            {
                if(theta == 180)
                {
                    max_radius = radial_width;
                    cout<<"Max Radius: "<<max_radius<<endl;
                }
                else
                {
                    max_radius = Common::abs_val((float)radial_width/cos(theta*PI/180.0));
                    cout<<"Max Radius: "<<max_radius<<endl;
                }
            }
            else if(theta <= 315.0)
            {
                if(theta == 270)
                {
                    max_radius = radial_height;
                    cout<<"Max Radius: "<<max_radius<<endl;
                }
                else
                {
                    max_radius = Common::abs_val((float)radial_height/sin(theta*PI/180.0));
                    cout<<"Max Radius: "<<max_radius<<endl;
                }
            }
            //}}}

            //binary search, but first check edge condition
            radius = max_radius;
            float jump_size = max_radius/2.0;

            int count = 0;
            while(!edge_found)
            {
                count++;
                //cout<<"\t->radius = "<<radius<<endl;
                Common::ThetaAndRadius_toXY(theta, radius, x, y);

                //offset the x and y
                x += radial_width;
                y = (-1.0 * y) + radial_height;
                //cout<<"\t->(x,y) = ("<<x<<","<<y<<")"<<endl;

                if(this->sprite_silhouette[x][y])
                {
                    //cout<<"Silhouette is true! ("<<count<<")"<<endl;
                    //is there any adjacent empty area (is this close to an edge?)
                    //check all 8 adjacent elements (with bounds checking)...really ugly
                    if(    !this->sprite_silhouette[ (x == width - 1) ? x : x+1 ]  [y] 
                        || !this->sprite_silhouette[(x == width - 1) ? x : x+1]  [ (y == height -1) ? y : y+1 ] 
                        || !this->sprite_silhouette[x]  [ (y == height -1) ? y : y+1 ] 
                        || !this->sprite_silhouette[(x > 0) ? x-1: 0]  [(y == height -1) ? y : y+1] 
                        || !this->sprite_silhouette[(x > 0) ? x-1: 0]  [y] 
                        || !this->sprite_silhouette[(x > 0) ? x-1: 0]  [(y > 0) ? y-1: 0]
                        || !this->sprite_silhouette[x]  [(y > 0) ? y-1: 0]
                        || !this->sprite_silhouette[(x == width - 1) ? x : x+1]  [(y > 0) ? y-1: 0])
                    {
                        edge_found = true;
                    }
                    else //no adjacent pixel is 0 (unset), not edge
                    {
                        //if this check is close to the max, and there is no edge here
                        //then the sprite may have data all along the radius at this theta
                        //consider radius here to be the max radius here
                        if( (max_radius - radius ) < 2)
                        {
                            radius = max_radius;
                            Common::ThetaAndRadius_toXY(theta, radius, x, y);
                            x += radial_width;
                            y = (-1.0 * y) + radial_height;
                            edge_found = true;
                        }
                        else
                        {
                            //move radius outward to find edge
                            last_radius = radius;
                            radius = radius + jump_size;
                           
                            //keep jump size from becoming to small
                            if(jump_size >= 1.0)
                                jump_size /= 2.0;
                        }
                    }
                }
                else
                {
                   last_radius = radius; 
                   radius = radius - jump_size;
                   if(jump_size >= 1.0)
                       jump_size /= 2.0;
                }
                if(count >100)
                {
                    edge_found = true;
                    cout<<"---------------No edge found!------"<<endl;
                }
            }
            edge_found = false;
            radius_sum += radius;
            //}}}
        }
        slices_cont.slice_radius.push_back((int)(radius_sum / num_checks_per_slice));
        radius_sum = 0.0;
  }

  for(int i = 0; i < slices_cont.slice_radius.size(); i ++)
      cout<<"Slice "<<i<<" = "<<slices_cont.slice_radius.at(i)<<endl;

  
    return slices_cont;
    //}}}
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

