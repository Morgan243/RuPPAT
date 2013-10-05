//represents a player  

#include "Player.h"
#include "PhysFuncs.h"

//secondary-CONSTRUCTOR: leave many attributes statically defined by literals,
//good for any quick tests
Player :: Player(string sprite_path,
                int num_rotations,
                int startingAngle) 
        : Object(sprite_path,
		       	200,
                200,
                1,
                num_rotations,
                startingAngle)
{
//{{{
	descriptor.xAcc = 0;
	descriptor.yAcc = 0;
	descriptor.maxAccel = 1;
	descriptor.mass=1000;
	descriptor.noDrawEffect = false;

	isSelected = true;

//}}}
}

//Constructor with more options
Player :: Player(string sprite_path,
	       	int num_rotations,
	       	int startingAngle,
			float maxaccel,
	       	int startX,
	       	int startY) 
	: Object(sprite_path,
			startX,
			startY,
			1,
			num_rotations,
			startingAngle)
{
//{{{
	descriptor.xAcc = 0;
	descriptor.yAcc = 0;
	descriptor.maxAccel = maxaccel;
	descriptor.noDrawEffect = false;

	isSelected = true;
//}}}
}

//Constructor with more options
Player :: Player(string sprite_path,
	       	int num_rotations,
	       	int startingAngle,
			float maxaccel,
	       	int startX,
	       	int startY,
	       	string HC_path) 
	: Object(sprite_path,
			startX,
			startY,
			1,
			num_rotations,
			startingAngle,
			0,
			0,
			HC_path)

{
//{{{
	descriptor.xAcc = 0;
	descriptor.yAcc = 0;
	descriptor.maxAccel = maxaccel;
	descriptor.noDrawEffect = false;

	isSelected = true;
//}}}
}

//Deconstructor 
Player :: ~Player()
{
//{{{
	for(int i = 0; i < missiles.size() ; i++)
	{
		delete(missiles[i]);
	}

	for(int i = 0; i < missiles_free.size() ; i++)
	{
		delete(missiles_free[i]);
	}
//}}}
}

//add a usable missile to the players arsenal
void Player :: addMissile(const string sprite_path,
                            const string name,
                            const bool makeSelected,
                            const int amnt,
                            const int mxDamage,
                            const float velocity,
                            const float lifespan)
{
//{{{
	cout<<"Adding missile:"<<endl;
	cout<<"Path:"<<sprite_path<<endl;
	cout<<"rots:"<<numRotations<<endl;

	//init and add missile
	missiles.push_back(
			new Missile(sprite_path, numRotations, 0, 10, 10.0, 
			descriptor.x, descriptor.y, descriptor.xVel, descriptor.yVel, sprite_path
			));

	missiles.back()->setName(name);

	missiles.back()->setAmount(amnt);

	missiles.back()->setDamage(mxDamage);

	missiles.back()->SetLifespan(lifespan);

	missiles.back()->SetLaunchForce(velocity);

	//not actually configurable yet, hard coded
	missiles.back()->SetLaunchForceDuration(1);

	if(makeSelected)
		selected_missile = missiles.back();
//}}}
}

void Player :: addMissile(const string sprite_path,
                            const string name,
                            const bool makeSelected,
                            const int amnt,
                            const int mxDamage)
{
//{{{
	cout<<"Adding missile:"<<endl;
	cout<<"Path:"<<sprite_path<<endl;
	cout<<"rots:"<<numRotations<<endl;

	missiles.push_back(
			new Missile(sprite_path, numRotations, 0, 1, 10.0, 
			descriptor.x, descriptor.y, descriptor.xVel, descriptor.yVel, sprite_path
			));

	missiles.back()->setName(name);

	missiles.back()->setAmount(amnt);

	missiles.back()->setDamage(mxDamage);

	if(makeSelected)
		selected_missile = missiles.back();
//}}}
}

bool Player :: updateSprite()
{
//{{{
	sprite->updateSprite();

	if(isSelected)
		return true;
	else
		return false;
//}}}
}

//select a missile and return a pointer to it
Missile* Player::selectMissile_byName(const string name)
{
//{{{
	for( int i = 0; i < missiles.size(); i++)
	{
		if(missiles[i]->getName() == name)
		{
			selected_missile = missiles[i];
			return selected_missile;
		}
	}
	cout<<"Error: Missile selection of "<<name<<" not found"<<endl;
	return NULL;
//}}}
}

//select a missile and return a pointer to it
Missile* Player::selectMissile_byIndex(const int index)
{
//{{{
	if((index<missiles.size()) && index >=0)
	{
		selected_missile = missiles[index];
		return selected_missile;
	}
	cout<<"Error: Missile selection at index ["<<index<<"] is invalid"<<endl;
	return NULL;
//}}}
}

Missile* Player::getSelectedMissile()
{
    return selected_missile;
}

bool Player :: areMissilesFree()
{
//{{{
	if(missiles_free.size())
		return true;
	else
		return false;
//}}}
}

vector<Missile*> Player :: getFreeMissiles()
{
	return missiles_free;
}

void Player::fireSelectedMissile()
{
//{{{	
		//create new missile: copy from selected 
		//Missile *firedMissile = new Missile(*selected_missile);
		Missile *firedMissile = selected_missile;// new Missile(*selected_missile);

		cout<<"\""<<selected_missile->getName()<<"\" fired!"<<endl;

		//get the descriptor from the new missile
		Entity_desc tempDesc = firedMissile->getDescriptor();

		//set the descriptors coordinates to the Player location
		tempDesc.x = descriptor.x;
		tempDesc.y = descriptor.y;

		//get velocity vectors of the player 
		GetVectors_FrontRelative(tempDesc.xVel,
			   						tempDesc.yVel,
									0.0, 120);

		//add velocity of player to missile
		tempDesc.xVel += descriptor.xVel;
		tempDesc.yVel += descriptor.yVel;

        cout<<"X velocity = "<< tempDesc.xVel <<endl;

		//update positional stats with descriptor: set descriptor
		firedMissile->setDescriptor(tempDesc);

		firedMissile->SetTimeCreated(thisTime);
		firedMissile->SetLifespan(1.0);

		//Missile is free, add it to vector of fired missiles
		missiles_free.push_back(firedMissile);

		//copy over reference to fired missiles descriptor
		this->to_render.entities.push_back(firedMissile->getDescriptor_ref());

		missiles_free.back()->setAngleIndex(this->sprite->getAngleIndex());
		
		//return firedMissile;
//}}}
}

void Player::fireSelectedMissile(Sprite* fired_sprite)
{
//{{{
    Entity_desc tempDesc = selected_missile->getDescriptor();

    //set the descriptors coordinates to the Player location
    tempDesc.x = descriptor.x;
    tempDesc.y = descriptor.y;

    //get velocity vectors of the player 
    GetVectors_FrontRelative(tempDesc.xVel,
                                tempDesc.yVel,
                                0.0, 120);

    //add velocity of player to missile
    tempDesc.xVel += descriptor.xVel;
    tempDesc.yVel += descriptor.yVel;
    tempDesc.xVel /= 2;
    tempDesc.yVel /= 2;
    tempDesc.xAcc = 20 * tempDesc.xVel;
    tempDesc.yAcc = 20 * tempDesc.yVel;

    cout<<"X Velocy = " << tempDesc.xVel<<endl;

    //create our new missile
    Missile *firedMissile = new Missile(fired_sprite,
                                        360, 0, tempDesc.mass,
                                        tempDesc.maxAccel,
                                        descriptor.x, descriptor.y,
                                        0,0, "");

    firedMissile->owning_object = this;

    //update positional stats with descriptor: set descriptor
    firedMissile->setDescriptor(tempDesc);

    firedMissile->SetTimeCreated(thisTime);
    firedMissile->SetLifespan(selected_missile->getLifespan());

    //Missile is free, add it to vector of fired missiles
    missiles_free.push_back(firedMissile);

    //copy over reference to fired missiles descriptor
    this->to_render.entities.push_back(firedMissile->getDescriptor_ref());

    missiles_free.back()->setAngleIndex(this->sprite->getAngleIndex());
//}}}
}

Entity_desc* Player::PhysicsHandler(const float t,
									const float dt,
									Entity_desc &state_src)
{
//{{{
	//size is how many missiles player has out and about
	int size = missiles_free.size();
	thisTime = t;

	//calculate players position
	PhysFunc::integrate(descriptor, t, dt, state_src);

	//calculate fired (free) missiles position
	for(int i = 0; i < size; i++)
	{
		//is the missile completely finished? (no longer relevant)
		if(missiles_free[i]->killMe)
		{
			//remove from the fired list
			missiles_free.erase(missiles_free.begin()+i);

			//decrement index and size
			i--;
			size--;
		}
		//time to begin end of life animation/sequence
		else if(missiles_free[i]->IsBeyondLifeSpan(thisTime) 
				&& !missiles_free[i]->isDestroying)
		{
			//initiate destruction
			missiles_free[i]->GameDestroy();
		}
		//integrate
		else 
		{
			missiles_free[i]->PhysicsHandler(t,
                                            dt,
                                            state_src);
		}
	}

	return &descriptor;
//}}}
}

Entity_desc* Player::PhysicsHandler(Entity_desc &state_dest, 
									const float t, 
									const float dt)
{
//{{{
	//set time
	thisTime = t;

	//integrate: apply gravitational force from this onto state_dest
	PhysFunc::integrate(state_dest, t, dt, descriptor);

	return &state_dest;	
//}}}
}

Entity_desc* Player::updatePositional(float t, float dt)
{
    PhysFunc::integrate_force(this->descriptor, t, dt);

	int size = missiles_free.size();

	for(int i = 0; i < size; i++)
        PhysFunc::integrate_force( *missiles_free[i]->getDescriptor_ref(), t, dt);
}

Entity_desc* Player :: stateUpdate(const float t,
                                            const float dt, 
                                            Entity_desc &state_src)
{
//{{{
    int size = missiles_free.size();
    //keep track of current time
	thisTime = t;

	//integrate the sprite location if its alive
	if(!isDestroying)
		PhysFunc::G_force(this->descriptor, state_src);

	//calculate fired (free) missiles position
	for(int i = 0; i < size; i++)
	{
		//is the missile completely finished? (no longer relevant)
		if(missiles_free[i]->killMe)
		{
			//remove from the fired list
			missiles_free.erase(missiles_free.begin()+i);

			//decrement index and size
			i--;
			size--;
		}
		//time to begin end of life animation/sequence
		else if(missiles_free[i]->IsBeyondLifeSpan(thisTime) 
				&& !missiles_free[i]->isDestroying)
		{
			//initiate destruction
			missiles_free[i]->GameDestroy();
            cout<<"!!!!!!!GAME DESTORY!!!!!!"<<endl;
		}
		//integrate
		else 
		{
			missiles_free[i]->stateUpdate(t,
                                            dt,
                                            state_src);
		}
	}
	return &descriptor;
//}}}
}

Entity_desc* Player :: stateUpdate(Entity_desc &state_dest, 
											const float t, 
											const float dt)
{
//{{{
	thisTime = t;
	
	//default for of this constructor
	PhysFunc::G_force(state_dest, descriptor);
	return &state_dest;
//}}}
}

bool Player :: GetNextAuxDrawInfo(Renderables_Cont &renderables)
{
//{{{
	//check bounds of the missiles_free vector
	if(refCounter < missiles_free.size())
	{
		//Call update functions and get what needs rendering
		missiles_free[refCounter]->UpdateAndGetRenderables(renderables);

		//increment the index: access next missile on next access
		refCounter++;
		return true;
	}
	//all done
	else
	{
		//reset refcount
		refCounter = 0;

		//indicate finished
		return false;
	}
//}}}
}

bool Player::GetRenderables(Renderables_Cont &Renderables_Cont)
{

}
