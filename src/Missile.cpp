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
	timeCreated = 0;//SDL_GetTicks();
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

bool Missile::IsBeyondLifeSpan()
{
//{{{
	if( (thisTime - timeCreated) > lifespan)
		return true;
	else
		return false;
//}}}
}

bool Missile::IsBeyondLifeSpan(float gameTime)
{
//{{{
	thisTime = gameTime;

	cout<<"Been alive: "<< (thisTime - timeCreated)<<endl;

	if( (thisTime - timeCreated) > lifespan)
		return true;
	else
		return false;
//}}}
}

Surface_Container Missile::UpdateAndGetRenderables(Renderables_Cont &rnder)
{
	this->updateSprite();

	Surface_Container ret_surface;
		ret_surface.x = descriptor.x;
		ret_surface.y = descriptor.y;
		ret_surface.surface = this->getSprite();

	rnder = to_render;

	return ret_surface;
}
