// 			[Class] Object
//--------------------------------------------------------------------
//| 	Object is the base class for moveable Entities. This is a 
//|intended as a parent class but is useable for basic moving sprites.
//--------------------------------------------------------------------
#ifndef OBJECT_H
#define OBJECT_H

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL2_rotozoom.h"
#include "SDL2/SDL_image.h"
#include "math.h"
#include "omp.h"

#include "Descriptors.h"
#include "Sprite.h"
#include "Common.h"

#include <vector>
#include <iostream>



//#include "PhysFuncs.h"
using namespace std;

class Object
{
	public:

		explicit Object(string sprite_path,
						int start_x,
						int start_y,
						int start_mass);

		explicit Object(string sprite_path,
						int start_x,
						int start_y,
						int start_mass,
						int num_rotations,
						int starting_angle);

		explicit Object(string sprite_path,
						int start_x,
						int start_y,
						int start_mass,
						int num_rotations,
						int starting_angle,
						float xVel,
						float yVel,
						string HC_path);

        explicit Object(Sprite* obj_sprite,
                        int start_x,
						int start_y,
						int start_mass);

		explicit Object(Sprite* obj_sprite,
						int start_x,
						int start_y,
						int start_mass,
						int num_rotations,
						int starting_angle);

		explicit Object(Sprite* obj_sprite,
						int start_x,
						int start_y,
						int start_mass,
						int num_rotations,
						int starting_angle,
						float xVel,
						float yVel,
						string HC_path);


		Object(const Object& src);

		Object &operator=(const Object &src);

		~Object();

        //Descriptor
        //{{{
		Entity_desc getDescriptor () const;

		Entity_desc* getDescriptor_ref();

		void setDescriptor(const Entity_desc new_desc);
		//}}}
        
        //Rotation
        //{{{
		void setRotationRate(const float rotRate);

		float getRotationRate();

		void incrementRotationRate();

		void decrementRotationRate();
        //}}}

        //Angle
        //{{{
		float getAngle();

		void setAngleIndex(const int angleIndex);

		void incrementAngleIndex();

		void decrementAngleIndex();
        //}}}
        
        //Sprite
        //{{{
		SDL_Surface *getSprite();

		SDL_Surface *getSprite(const int angle);

		SDL_Texture *getSprite_text();

		SDL_Texture *getSprite_text(const int angle);

		virtual bool updateSprite();
        //}}}

        //Attributes
        //{{{
		void setID(const int id);

		int getX ()const;

		int getY ()const;

		void setXY(const int x, const int y);

		int getMass ()const;

		float getXvel ()const;

		float getYvel ()const;

		float getXacc ()const;

		float getYacc ()const;
        //}}}

		void accelForward();	

		void accelBackward();

		void setAccelVectors(const bool forward);

        void setForceVectors(const bool forward);

		void getExhaustVectors(float &xVel, float &yVel);

		void GetVectors_FrontRelative(float &xVect,
										float &yVect,
										const float angleFromFront,
										const float mag);


		void buildHitBoxesFromSprite();

		void buildHitBoxesFromLayer(SDL_Surface* hitBoxLayout);

		bool checkHits(float &xVel,
						float &yVel,
						Entity_desc &check_ent);

		float lastErr;

		float rotationalErrorAccum;

		virtual Entity_desc* PhysicsHandler(const float t,
											float dt,
											Entity_desc &state_src);

		virtual Entity_desc*  PhysicsHandler(Entity_desc &state_dest, 
												const float t,
												const float dt);

        virtual Entity_desc* updatePositional(float t, float dt);

		virtual Entity_desc* stateUpdate(const float t,
                                                    float dt,
                                                    Entity_desc &state_src);

		virtual Entity_desc*  stateUpdate(Entity_desc &state_dest, 
                                                    const float t,
                                                    const float dt);


		virtual bool GetRenderables(Renderables_Cont &renderables);

		virtual Renderables_Cont* GetRenderables();

		virtual bool UpdateAndGetRenderables(Renderables_Cont rnder);

		//destroy the object in game (destruction animation/effects
		//initialization begins here) not a destructor operation
		virtual void GameDestroy();

		float setTimeNow(const float time);

		float getTime();

		Sprite *sprite, hitCircleSprite;

		bool killMe,
			 isDestroying;

        Object* owning_object;

	protected:
		static int next_obj_id;
		int refMax, 
			refCounter,
		   	numRotations;

		float exhaustX, 
			  exhaustY,
			  timeCreated;

		Entity_desc descriptor;

		//generic timestamp for class
		float thisTime, last_update_t;

		Renderables_Cont to_render;

		vector<Pixel_desc> pixelSprite_cache;
};
#endif
