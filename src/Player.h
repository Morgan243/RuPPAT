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

		void setRotation_rate(float rotRate);

		float getRotation_rate();

		void incrementRotation_rate();

		void decrementRotation_rate();

		void updateSprite();

		float getAngle();

		void setAngle_index(int angleIndex);

		void incrementAngle_index();

		void decrementAngle_index();

		SDL_Surface * getSprite();

		SDL_Surface * getSprite(int angle);

//----v_v_v_DESCRIPTOR MANAGEMENT_v_v_v------
		void setDescriptor(Entity_desc new_desc);
		Entity_desc getDescriptor();

		void accelForward();	
		void accelBackward();

		void setAccelVectors(bool forward);

		void getXY_exhaust(float &xVel, float &yVel);

		float lastErr;
		float rotationalErrorAccum;
	protected:
	
		float DEGREE_INCREMENT;
		int currentAngle_index;
		float currentAngle_index_f;

		float rotationRate;

		float exhaustX, exhaustY;
	
		Sprite sprite;

		vector<SDL_Surface *> rotations;
};
