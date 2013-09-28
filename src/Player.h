//			[Class] Player 
//--------------------------------------------------------------------
//| 	This objects represents a Player; any entity that is to be 
//|controlled by a person/player or AI. Derived from Object.
//--------------------------------------------------------------------
#ifndef PLAYER_H
#define PLAYER_H
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL2_rotozoom.h"
#include "SDL2/SDL_image.h"
#include "math.h"

#include "Object.h"
#include "Missile.h"

#include <vector>
#include <iostream>


using namespace std;
		
//Player inherits from object
class Player : public Object
{
	public:

		explicit Player(string sprite_path, 
						int num_rotations,
						int startingAngle);

		explicit Player(string sprite_path,
						int num_rotations,
						int startingAngle,
						float maxaccel,
						int startX,
						int startY);

		explicit Player(string sprite_path,
						int num_rotations,
						int startingAngle,
						float maxaccel,
						int startX,
						int startY, 
						string HC_path);
		~Player();


		bool updateSprite();

		//add a missile, auto sets the sprite path to the 
		//default sprite
		void addMissile(const string sprite_path,
						const string name,
						const bool makeSelected,
						const int amnt,
						const int mxDamage,
						const float velocity,
						const float lifespan);

		//load sprite and create a new Missile object
		void addMissile(const string sprite_path,
						const string name,
						const bool makeSelected,
						const int amnt,
						const int mxDamage);
		
		Missile* selectMissile_byName(const string name);

		Missile* selectMissile_byIndex(const int index);
	
		Missile* getSelectedMissile();

		//check to see if engine needs to handle
		//weapons physics,collisions, etc
		bool areMissilesFree();

		//return all missiles that have been launched
		vector<Missile*> getFreeMissiles();

		//launch a missile!
		void fireSelectedMissile();
		void fireSelectedMissile(Sprite* fired_sprite); 

        Entity_desc* updatePositional(float t, float dt);

		Entity_desc *PhysicsHandler(const float t,
									const float dt,
									Entity_desc &state_src);
		
		Entity_desc *PhysicsHandler(Entity_desc &state_dest,
									const float t,
									const float dt);

        Entity_desc* stateUpdate(const float t,
                                            const float dt, 
                                            Entity_desc &state_src);

        Entity_desc* stateUpdate(Entity_desc &state_dest, 
											const float t, 
											const float dt);
		
		vector<Entity_desc*>* GetAuxillaryDescriptors();

		bool GetNextAuxDrawInfo(Renderables_Cont &renderables);

		bool GetRenderables(Renderables_Cont &renderables);

	protected:
		//what missile will be fired if trigger is pulled
		Missile* selected_missile;

		//missiles usable by player
		vector <Missile*> missiles;

		//Missiles free, or fired, by player
		//allows multiple missles to be launched at once
		vector<Missile*> missiles_free;

		//Any "extra" descriptors: missiles, effects
		vector<Entity_desc*> auxillary_desc;

		//User has selected this unit; not currently in use
		bool isSelected;
};
#endif
