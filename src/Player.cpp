//represents a player  

#include "Player.h"

//secondary-CONSTRUCTOR: leave many attributes statically defined by literals,
//good for any quick tests
Player :: Player(string sprite_path, int num_rotations, int startingAngle) 
	: Object(sprite_path, 200,200,1,num_rotations,startingAngle)
	//, sprite(sprite_path, num_rotations, startingAngle)
//{{{
{
	descriptor.xAcc = 0;
	descriptor.yAcc = 0;
	descriptor.maxAccel = 1;
	descriptor.mass=1000;
	descriptor.noDrawEffect = false;

}
//}}}

//Constructor with more options
Player :: Player(string sprite_path, int num_rotations, int startingAngle,
		float maxaccel, int startX, int startY) 
		: Object(sprite_path,startX,startY,1,num_rotations,startingAngle)
//		, sprite(sprite_path,num_rotations,startingAngle)
//{{{
{
	descriptor.xAcc = 0;
	descriptor.yAcc = 0;
	descriptor.maxAccel = maxaccel;
	descriptor.noDrawEffect = false;
}
//}}}

//Constructor with more options
Player :: Player(string sprite_path, int num_rotations, int startingAngle,
		float maxaccel, int startX, int startY, string HC_path) 
		: Object(sprite_path,startX,startY,1, num_rotations,startingAngle,0,0,HC_path)
	//, sprite(sprite_path, num_rotations,startingAngle)
//{{{
{
	descriptor.xAcc = 0;
	descriptor.yAcc = 0;
	descriptor.maxAccel = maxaccel;
	descriptor.noDrawEffect = false;
}
//}}}

//Deconstructor 
Player :: ~Player()
{


}


