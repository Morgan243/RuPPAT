#include "Missile.h"

Missile::Missile(string sprite_path, int num_rotations, int startingAngle, int mass,
					float maxaccel, int startX, int startY,
					float xVel, float yVel, string HC_path)
	:Object(sprite_path, startX, startY, mass, num_rotations, 
			startingAngle,xVel,yVel,HC_path)
{

	timeCreated = SDL_GetTicks();
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

void Missile::SetLifespan(int seconds)
{
	lifespan = seconds * 1000;//convert to ms
}

bool Missile::IsBeyondLifeSpan()
{
//{{{
	if( (SDL_GetTicks() - timeCreated) > lifespan)
	{
		return true;
	}else
		return false;
//}}}
}
