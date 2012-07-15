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

#include <vector>
#include <iostream>

using namespace std;

class Player
{
	public:
		Player(string sprite_path, int num_rotations, int startingAngle);
		Player(string sprite_path, int num_rotations, int startingAngle,
					float maxaccel, int startX, int startY);
		~Player();

		void setRotation_rate(int rotRate);

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

		int getX();
		int getY();

		float getXvel();
		float getYvel();

		float getXacc();
		float getYacc();

		void accelForward();	
		void accelBackward();

		void setAccelVectors(bool forward);

		void getXY_exhaust(float &xVel, float &yVel);

		float lastErr;
		float rotationalErrorAccum;
	private:
	
		float DEGREE_INCREMENT;
		int currentAngle_index;
		float currentAngle_index_f;

		float x, y;
		float xVel, yVel;//velocity vector
		float xAcc, yAcc;//acceleration vector
		int xGacc,yGacc; //grav acceleration
	
		int mass; //mass	

		float rotationRate;

		float exhaustX, exhaustY;
		Entity_desc descriptor;

		vector<SDL_Surface *> rotations;
};
