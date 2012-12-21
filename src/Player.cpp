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
//{{{
{
	descriptor.xAcc = 0;
	descriptor.yAcc = 0;
	descriptor.maxAccel = 1;
	descriptor.mass=1000;
	descriptor.noDrawEffect = false;

	isSelected = true;

}
//}}}

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
//{{{
{
	descriptor.xAcc = 0;
	descriptor.yAcc = 0;
	descriptor.maxAccel = maxaccel;
	descriptor.noDrawEffect = false;

	isSelected = true;
}
//}}}

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
//{{{
{
	descriptor.xAcc = 0;
	descriptor.yAcc = 0;
	descriptor.maxAccel = maxaccel;
	descriptor.noDrawEffect = false;

	isSelected = true;
}
//}}}

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

void Player :: addMissile(string sprite_path,
	       			string name,
			       	bool makeSelected,
				int amnt,
			       	int mxDamage,
			       	float velocity,
			       	float lifespan)
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

	missiles.back()->SetLifespan(lifespan);

	missiles.back()->SetLaunchForce(velocity);

	//not actually configurable yet, hard coded
	missiles.back()->SetLaunchForceDuration(1);

	if(makeSelected)
		selected_missile = missiles.back();
//}}}
}



void Player :: addMissile(string sprite_path,
	       			string name,
			       	bool makeSelected,
				int amnt,
			       	int mxDamage)
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
	sprite.updateSprite();

	if(isSelected)
		return true;
	else
		return false;
//}}}
}

//select a missile and return a pointer to it
Missile* Player::selectMissile_byName(string name)
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
Missile* Player::selectMissile_byIndex(int index)
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

Missile* Player::fireSelectedMissile()
{
//{{{	

		Missile *firedMissile = new Missile(*selected_missile);

		cout<<"\""<<selected_missile->getName()<<"\" fired!"<<endl;
		cout<<"\tx,y = "<<descriptor.x<<" , "<<descriptor.y<<endl;

		//get the descriptor from the new missile
		Entity_desc tempDesc = firedMissile->getDescriptor();

		//set the descriptors coordinates to the Player location
		tempDesc.x = descriptor.x;
		tempDesc.y = descriptor.y;

		//get velocity vectors using 
		GetVectors_FrontRelative(tempDesc.xVel, tempDesc.yVel,
						0.0, 120);

		//add velocity of player to missile
		tempDesc.xVel += descriptor.xVel;
		tempDesc.yVel += descriptor.yVel;

		//update positional stats with descriptor
		firedMissile->setDescriptor(tempDesc);

		firedMissile->SetTimeCreated(thisTime);
		firedMissile->SetLifespan(1.0);
		//firedMissile->setTimeNow();
		//setTimeNow();

		//Missile is free, add it to vector
		missiles_free.push_back(firedMissile);

		//copy over reference to fired missiles descriptor
		//auxillary_desc.push_back(firedMissile->getDescriptor_ref());
		this->to_render.entities.push_back(firedMissile->getDescriptor_ref());
			

		return firedMissile;
//}}}
}

Entity_desc* Player::PhysicsHandler(float t,
	       				float dt,
					Entity_desc &state_src)
{
//{{{
	int size = missiles_free.size();
	thisTime = t;

	//cout<<"time: "<<thisTime<<endl;
	//calculate players position
	PhysFunc::integrate(descriptor, t, dt, state_src);

	//calculate fire or free missiles position
	for(int i = 0; i < size; i++)
	{
		if(missiles_free[i]->killMe)
		{
			//cout<<"Missile being killed!"<<endl;
			missiles_free.erase(
					missiles_free.begin()+i);
			i--;
			size--;
		}
		else if(missiles_free[i]->IsBeyondLifeSpan(thisTime) 
				&& !missiles_free[i]->isDestroying)
		{
			cout<<"Missile being game destroyed!"<<endl;
			missiles_free[i]->GameDestroy();
		}
		else 
			//cout<<"Just handling physics!"<<endl;
			missiles_free[i]->PhysicsHandler(t,
							dt,
						       	state_src);
	}

//	for(int i = 0; i < to_render.pixels.size(); i++)
//	{
//		PhysFunc::integrate(to_render.pixels[i], t, dt, state_src);
//	}

	return &descriptor;
//}}}
}

Entity_desc* Player::PhysicsHandler( Entity_desc &state_dest, 
					float t, 
					float dt)
{
//{{{
	thisTime = t;
	PhysFunc::integrate(state_dest, t, dt, descriptor);
	return &state_dest;	
//}}}
}


vector<Entity_desc*>* Player :: GetAuxillaryDescriptors()
{
	//return &auxillary_desc;	
	return &this->to_render.entities;
}

SDL_Surface* Player :: GetNextAuxDrawInfo(int &x,
	       				int &y,
				       	SDL_Surface* &refSurf,
					Renderables_Cont &renderables)
{
//{{{
	refCounter++;
	if(refCounter < missiles_free.size())
	{

		Surface_Container tmpSurfCon
			= missiles_free[refCounter]->
						UpdateAndGetRenderables(renderables);
		refSurf = tmpSurfCon.surface;
		x = tmpSurfCon.x;
		y = tmpSurfCon.y;
		

		return refSurf;
	}
	else
	{
		refCounter = -1;
		return NULL;
	}
//}}}
}


SDL_Surface* Player :: GetNextAuxDrawInfo(Renderables_Cont &renderables)
{
//{{{
	refCounter++;
	if(refCounter < missiles_free.size())
	{

		Surface_Container tmpSurfCon = missiles_free[refCounter]->
						UpdateAndGetRenderables(renderables);

		renderables.sprites.push_back(tmpSurfCon);

		return tmpSurfCon.surface;
	}
	else
	{
		refCounter = -1;
		return NULL;
	}
//}}}
}



bool Player::GetRenderables(Renderables_Cont &Renderables_Cont)
{

}
