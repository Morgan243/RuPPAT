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
		
		void setName(string name);

		string getName();

		void setAmount(int amnt);

		int incrementAmount(int incrBy);

		int getAmount();

		void setDamage(int dmg);

		int incrementDamage(int incrBy);

		int getDamage();

		void SetLaunchForce(int force);

		void SetLaunchForceDuration(int duration);

		void SetTimeCreated(float gameTime);

		void SetLifespan(float seconds);

		bool IsBeyondLifeSpan();

		bool IsBeyondLifeSpan(float gameTime);

		bool UpdateAndGetRenderables(
						Renderables_Cont &rnder);

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
