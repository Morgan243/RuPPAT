#include "Missile.h"

Missile::Missile(string sprite_path, int num_rotations, int startingAngle, int mass,
					float maxaccel, int startX, int startY,
					float xVel, float yVel, string HC_path)
	:Object(sprite_path, startX, startY, mass, num_rotations, 
			startingAngle,xVel,yVel,HC_path)
{


}
