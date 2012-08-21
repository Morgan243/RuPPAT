/* 	Missile
 *  This object represent the missile wepon 
 *  in action. It derives from the object class
 *  and adds functionality present in projectile
 *  based weapons
 */

#include "Descriptors.h"
#include "Object.h"
#include "Sprite.h"

#include <vector>
#include <iostream>

using namespace std;

//Missile inherits from object
class Missile : public Object
{
	public:
		explicit Missile(string sprite_path, int num_rotations, int startingAngle, int mass,
					float maxaccel, int startX, int startY,
					float xVel, float yVel, string HC_path);		
		~Missile();

	protected:

		int damage;

};
