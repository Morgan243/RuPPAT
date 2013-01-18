// 			[Class]	Missile
//--------------------------------------------------------------------
//| 	This object represent the missile weapon in action. It derives from
//|the object class and adds functionality present in projectile
//|based weapons.
//--------------------------------------------------------------------

#include "Object.h"

#include <vector>
#include <iostream>

#ifndef MISSILE_H
#define MISSILE_H
using namespace std;

//Missile inherits from object
class Missile : public Object
{
	public:
		explicit Missile(string sprite_pate,
			       		int num_rotations,
				       	int startingAngle, 
						int mass,
				       	float maxaccel,
				       	int startX,
				       	int startY,
						float xVel,
				       	float yVel,
				       	string HC_path);		
		~Missile();
		
		void setName(const string name);

		string getName();

		void setAmount(const int amnt);

		int incrementAmount(const int incrBy);

		int getAmount();

		void setDamage(const int dmg);

		int incrementDamage(const int incrBy);

		int getDamage();

		void SetLaunchForce(const int force);

		void SetLaunchForceDuration(const int duration);

		void SetTimeCreated(const float gameTime);

		void SetLifespan(const float seconds);

		bool IsBeyondLifeSpan();

		bool IsBeyondLifeSpan(const float gameTime);

		bool UpdateAndGetRenderables(Renderables_Cont &rnder);

	protected:
		//how many of this missile do we have
		int amount,
			damage,	//whats the max damage
			launchForce,//launch force (actually aceleration...for now)
			launchForceDuration;//how long to apply the launch force
		
		float lifespan;//how long will it stick around, in seconds

		string name;
};
#endif
