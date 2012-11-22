//Object is the base class for moveable Entities
//
//
//This is a parent class but is useable for basics 
#ifndef OBJECT_H
#define OBJECT_H

#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"
#include "math.h"

#include "Descriptors.h"
#include "Sprite.h"
//#include "PhysFuncs.h"

#include <vector>
#include <iostream>



//#include "PhysFuncs.h"
using namespace std;

class Object
{
	public:

explicit Object(string sprite_path, int start_x, int start_y, int start_mass);

explicit Object(string sprite_path, int start_x, int start_y, int start_mass,
			int num_rotations, int starting_angle);

explicit Object(string sprite_path, int start_x, int start_y, int start_mass,
			int num_rotations, int starting_angle,
				 float xVel, float yVel, string HC_path);

		Object(const Object& src);
		Object &operator=(const Object &src);

		~Object();

		Entity_desc getDescriptor ()const;
		Entity_desc* getDescriptor_ref();

		void setDescriptor(Entity_desc new_desc);
			
		SDL_Surface *getSprite();
		SDL_Surface *getSprite(int angle);


		void setID(int id);
		int getX ()const;
		int getY ()const;

		void setXY(int x, int y);

		int getMass ()const;

		float getXvel ()const;
		float getYvel ()const;

		float getXacc ()const;
		float getYacc ()const;

		void accelForward();	
		void accelBackward();

		void setAccelVectors(bool forward);

		void getXY_exhaust(float &xVel, float &yVel);

		void GetVectors_FrontRelative(float &xVect, float &yVect, 	//reference return
						float angleFromFront, float mag);

		void setRotation_rate(float rotRate);

		float getRotation_rate();

		void incrementRotation_rate();

		void decrementRotation_rate();

		virtual bool updateSprite();

		float getAngle();

		void setAngle_index(int angleIndex);

		void incrementAngle_index();

		void decrementAngle_index();

		void buildHitBoxes_fromSprite();

		void buildHitBoxes_fromLayer(SDL_Surface* hitBoxLayout);

		bool checkHits(float &xVel, float &yVel, Entity_desc &check_ent);


		Sprite sprite, hitCircleSprite;

		float lastErr;
		float rotationalErrorAccum;

		virtual Entity_desc* PhysicsHandler(float t, float dt,
						Entity_desc &state_src);

		virtual Entity_desc*  PhysicsHandler(Entity_desc &state_dest, 
							float t, float dt);

		virtual vector<Entity_desc*>* GetAuxillaryDescriptors();

	protected:
		int refMax, refCounter;
		int numRotations;
		float exhaustX, exhaustY;
		Entity_desc descriptor;

		unsigned int timeCreated;
};
#endif
