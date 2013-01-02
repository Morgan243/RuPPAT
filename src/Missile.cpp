#include "Missile.h"

Missile::Missile(string sprite_path, 
			int num_rotations,
	       	int startingAngle,
	       	int mass,
			float maxaccel,
	       	int startX, 
			int startY,
			float xVel,
	       	float yVel,
	       	string HC_path)

	:Object(sprite_path,
			startX,
	       	startY,
	       	mass,
	       	num_rotations, 
			startingAngle,
			xVel,
			yVel,
			HC_path)
{
	killMe = false;
	timeCreated = 0;
	cout<<"created at:"<<timeCreated<<endl;
}

Missile :: ~Missile()
{
}

void Missile :: setName(string name)
{
	this->name = name;
}

string Missile :: getName()
{
	return name;
}

void Missile::setAmount(int amnt)
{
	amount = amnt;
}

int Missile::incrementAmount(int incrBy)
{
	amount += incrBy;
	return amount;
}

int Missile::getAmount()
{
	return amount;
}

void Missile::setDamage(int dmg)
{
	damage = dmg;
}

int Missile::incrementDamage(int incrBy)
{
	damage += incrBy;
	return damage;
}

int Missile::getDamage()
{
	return damage;
}

void Missile::SetLaunchForce(int force)
{
	launchForce = force;
}	

void Missile::SetLaunchForceDuration(int duration)
{
	launchForceDuration = duration;
}

void Missile::SetTimeCreated(float gameTime)
{
	timeCreated = gameTime;
}

void Missile::SetLifespan(float gameTime)
{
	lifespan = gameTime;
}

//check lifespan
bool Missile::IsBeyondLifeSpan()
{
//{{{
	if( (thisTime - timeCreated) > lifespan)
		return true;
	else
		return false;
//}}}
}

//Set time and check lifespan 
bool Missile::IsBeyondLifeSpan(float gameTime)
{
//{{{
	thisTime = gameTime;

	if( (thisTime - timeCreated) > lifespan)
		return true;
	else
		return false;
//}}}
}

bool Missile::UpdateAndGetRenderables(Renderables_Cont &rnder)
{
//{{{
	this->updateSprite();

	rnder = to_render;

	//only add the sprite if the missile is active
	if(!isDestroying)
	{
		Surface_Container tmpSurfCont;
		tmpSurfCont.surface = this->getSprite();
		tmpSurfCont.x = descriptor.x;
		tmpSurfCont.y = descriptor.y;
		rnder.sprites.push_back(tmpSurfCont);

		return true;
	}

	return false;
//}}}
}
