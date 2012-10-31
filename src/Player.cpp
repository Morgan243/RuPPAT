//represents a player  

#include "Player.h"
#include "PhysFuncs.h"

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

void Player :: addMissile(string sprite_path, string name, bool makeSelected,
				int amnt, int mxDamage)
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
	Entity_desc tempDesc;
		tempDesc.ID = 0;
		tempDesc.x = descriptor.x;
		tempDesc.y = descriptor.y;

	Missile *firedMissile = new Missile(*selected_missile);
	cout<<"\""<<selected_missile->getName()<<"\" fired!"<<endl;
	cout<<"\tx,y = "<<descriptor.x<<" , "<<descriptor.y<<endl;
	//firedMissile->setDescriptor(tempDesc);
	firedMissile->setXY(descriptor.x, descriptor.y);
	missiles_free.push_back(firedMissile);
	auxillary_desc.push_back(firedMissile->getDescriptor_ref());

	return firedMissile;
}

Entity_desc* Player::PhysicsHandler(float t, float dt,
					Entity_desc &state_src)
{
	//calculate players position
	PhysFunc::integrate(descriptor, t, dt, state_src);

	//calculate fire or free missiles position
	for(int i = 0; i< missiles_free.size(); i++)
	{
		missiles_free[i]->PhysicsHandler(t, dt, state_src);
	}
	return &descriptor;
}

Entity_desc* Player::PhysicsHandler( Entity_desc &state_dest, 
					float t, float dt)
{
	PhysFunc::integrate(state_dest, t, dt, descriptor);
	return &state_dest;	
}


vector<Entity_desc*>* Player :: GetAuxillaryDescriptors()
{
	return &auxillary_desc;	
}

SDL_Surface* Player :: GetNextAuxDrawInfo(int &x, int &y, SDL_Surface *refSurf)
{
//{{{
	refCounter++;
	if(refCounter < missiles_free.size())
	{
		missiles_free[refCounter]->updateSprite();
		
		refSurf = missiles_free[refCounter]->getSprite();

		x = missiles_free[refCounter]->getX();
		y = missiles_free[refCounter]->getY();
		return refSurf;
	}
	else
	{
		refCounter = -1;
		return NULL;
	}
//}}}
}
