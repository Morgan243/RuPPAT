#include "Missile.h"

Missile::Missile(string sprite_path, int num_rotations, int startingAngle, int mass,
					float maxaccel, int startX, int startY,
					float xVel, float yVel, string HC_path)
	:Object(sprite_path, startX, startY, mass, num_rotations, 
			startingAngle,xVel,yVel,HC_path)
{


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



