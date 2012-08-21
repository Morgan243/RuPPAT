/*		Player 
 *	this objects represents a Player; any entity that that is 
 * to be controlled by a person/player or AI.
 *
 */
#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"
#include "math.h"

#include "Descriptors.h"
#include "Object.h"
#include "Sprite.h"

#include <vector>
#include <iostream>

using namespace std;

//Player inherits from object
class Player : public Object
{
	public:
		explicit Player(string sprite_path, int num_rotations, int startingAngle);

		explicit Player(string sprite_path, int num_rotations, int startingAngle,
					float maxaccel, int startX, int startY);

		explicit Player(string sprite_path, int num_rotations, int startingAngle,
					float maxaccel, int startX, int startY, string HC_path);
		~Player();


	protected:

};
