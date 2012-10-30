#ifndef PHYS_FUNCS_H
#define PHYS_FUNCS_H

#include <math.h>
#include <iostream>
#include "Descriptors.h"
//#include "Object.h"


namespace PhysFunc
{
float getDistance(float x1, float y1, float x2, float y2);

//need to decide how to apply all the forces to the object here
float G_acceleration (const Pixel_desc &state, Derivative &output, float t,
			const float objMass, const float objX, const float objY);

Derivative evaluate(Pixel_desc &initial,float t, float dt,const Derivative &d,
			const float objMass, const float point_x, const float point_y);

void integrate (Pixel_desc &state, float t, float dt, 
			const float objMass, const float point_x, const float point_y );


//-------------------vvvv GENERALIZED FOR ENTITIES vvvvv------------

//need to decide how to apply all the forces to the object here
float G_acceleration_ent (const Entity_desc &state, Derivative &output, float t,
			const float objMass, const float objX, const float objY);

Derivative evaluate_ent(Entity_desc &initial,float t, float dt,const Derivative &d,
				const float objMass, const float point_x, const float point_y);


void integrate_ent (Entity_desc &state, float t, float dt, 
			const float objMass, const float point_x, const float point_y );


//-------------------vvvv GENERALIZED FOR OBJECTS vvvvv------------
Derivative evaluate(Entity_desc &initial,float t, float dt,const Derivative &d,
			const Entity_desc &init_src);
	//const float objMass, const float point_x, const float point_y)


void integrate (Entity_desc &state, float t, float dt, 
			Entity_desc &state_src);
		//	Object &state_src)	
		//const float objMass, const float point_x, const float point_y )
}
#endif
