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

Missile::Missile(string sprite_path, 
                int num_rotations,
                int startingAngle,
                Entity_desc ent,
                string HC_path)
        :Object(sprite_path,
                ent.x,
                ent.y,
                ent.mass,
                num_rotations, 
                startingAngle,
                ent.xVel,
                ent.yVel,
                HC_path)
{
	killMe = false;
	timeCreated = 0;
	cout<<"created at:"<<timeCreated<<endl;
}

Missile::Missile(Sprite* obj_sprite,
                int num_rotations,
                int startingAngle,
                int mass,
                float maxaccel,
                int startX, 
                int startY,
                float xVel,
                float yVel,
                string HC_path)

        :Object(obj_sprite,
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

Missile::Missile(Sprite* obj_sprite,
                int num_rotations,
                int startingAngle,
                Entity_desc ent,
                string HC_path)

        :Object(obj_sprite,
                ent.x,
                ent.y,
                ent.mass,
                num_rotations, 
                startingAngle,
                ent.xVel,
                ent.yVel,
                HC_path)
{
	killMe = false;
	timeCreated = 0;
	cout<<"created at:"<<timeCreated<<endl;
}

Missile :: ~Missile()
{
}

void Missile :: setName(const string name)
{
	this->name = name;
}

string Missile :: getName()
{
	return name;
}

void Missile::setAmount(const int amnt)
{
	amount = amnt;
}

int Missile::incrementAmount(const int incrBy)
{
	amount += incrBy;
	return amount;
}

int Missile::getAmount()
{
	return amount;
}

void Missile::setDamage(const int dmg)
{
	damage = dmg;
}

int Missile::incrementDamage(const int incrBy)
{
	damage += incrBy;
	return damage;
}

int Missile::getDamage()
{
	return damage;
}

void Missile::SetLaunchForce(const int force)
{
	launchForce = force;
}	

void Missile::SetLaunchForceDuration(const int duration)
{
	launchForceDuration = duration;
}

void Missile::SetTimeCreated(const float gameTime)
{
	timeCreated = gameTime;
}

void Missile::SetLifespan(const float gameTime)
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
bool Missile::IsBeyondLifeSpan(const float gameTime)
{
//{{{
	thisTime = gameTime;

	if( (thisTime - timeCreated) > (float)lifespan)
    {
        cout<<"time created: " << timeCreated<<endl;
        cout<<"This Time: "<<thisTime<<endl;
        cout<<"--->Lifespan: "<<lifespan<<endl;
        cout<<"--->IsDestroying? : "<<this->isDestroying<<endl;
		return true;
    }
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
        Texture_Container tmpTextCont;
		Surface_Container tmpSurfCont;

		tmpSurfCont.surface = this->getSprite();
        tmpTextCont.texture = this->getSprite_text();

		tmpTextCont.x = tmpSurfCont.x = descriptor.x;
		tmpTextCont.y = tmpSurfCont.y = descriptor.y;

		rnder.sprites.push_back(tmpSurfCont);
        rnder.sprites_text.push_back(tmpTextCont);

		return true;
	}

	return false;
//}}}
}

float Missile::getLifespan()
{
    return this->lifespan;
}
