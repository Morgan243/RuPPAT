/*		Player 
 *	this objects represents a Player; any entity that that is 
 * to be controlled by a person/player or AI.
 *
 */

#ifndef PLAYER_H
#define PLAYER_H
#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"
#include "math.h"

//#include "PhysFuncs.h"
//#include "Descriptors.h"
#include "Object.h"
//#include "Sprite.h"
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
		void addMissile(string sprite_path,
			       	string name,
			       	bool makeSelected,
				int amnt,
			       	int mxDamage,
			       	float velocity,
			       	float lifespan);

		//load sprite and create a new Missile object
		void addMissile(string sprite_path,
			       	string name,
			       	bool makeSelected,
				int amnt,
			       	int mxDamage);
		
		Missile* selectMissile_byName(string name);

		Missile* selectMissile_byIndex(int index);
	
		Missile* getSelectedMissile();

		//check to see if engine needs to handle
		//weapons physics,collisions, etc
		bool areMissilesFree();

		//return all missiles that have been launched
		vector<Missile*> getFreeMissiles();

		//launch a missile!
		Missile* fireSelectedMissile();
		
		Entity_desc *PhysicsHandler(Entity_desc &state_dest,
						float t,
					       	float dt);

		Entity_desc *PhysicsHandler(float t,
			       			float dt,
						Entity_desc &state_src);
		
		vector<Entity_desc*>* GetAuxillaryDescriptors();

		SDL_Surface* GetNextAuxDrawInfo(int &x,
			       			int &y,
					       	SDL_Surface* &refSurf,
					Renderables_Cont &renderables);

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
