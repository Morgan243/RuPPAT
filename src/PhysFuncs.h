#include <math.h>
#include <iostream>
#include "Descriptors.h"

#ifndef PHYS_FUNCS_H
#define PHYS_FUNCS_H



struct Derivative
{
	//velocities
	float dx;
	float dy;
	
	//accelerations
	float ddx;
	float ddy;

};

float getDistance(float x1, float y1, float x2, float y2)
{
//std::cout<<"\t\tdistance Calc: x2="<<x2<<" x1="<<x1<<" y2="<<y2<<" y1="<<y1<<std::endl;
	return ( sqrt( ((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1))  ) );
}


//need to decide how to apply all the forces to the object here
float G_acceleration (const Pixel_desc &state, Derivative &output, float t,
			const float objMass, const float objX, const float objY)
{

	const float gravConst = 10;

	float distance = (float)getDistance(objX, objY, state.x, state.y);
	float theta, gAccTotal=0.0;

	float x = (float)state.x;
	float y = (float)state.y;

	float deltaX = objX - x;
	float deltaY = objY - y;
		
		if(deltaY!=0 && deltaX!=0)	
			 theta = atan((deltaX)/(deltaY));
		else if(deltaY== 0 && deltaX!=0)
			 theta =3.1415926/2.0;
		else if(deltaY!= 0 && deltaX==0)
			 theta = 0.0;


		if((deltaX<0 && deltaY<0) || (deltaX>0 && deltaY<0))
			 {theta *=(-1.0);}

		if(distance!=0 && distance>41 )
			 gAccTotal = (objMass*gravConst)/(distance*distance);


			output.ddx = gAccTotal * sin(theta);
			output.ddy = gAccTotal * cos(theta);

			if(y>objY){output.ddy *=-1;}

	return 0.0;
}

Derivative evaluate(Pixel_desc &initial,float t, float dt,const Derivative &d,
			const float objMass, const float point_x, const float point_y)
{
	Pixel_desc state;
	
	//calc new positions
	state.x = initial.x + d.dx * dt; 
	state.y = initial.y + d.dy * dt; 

	//calc new velocities
	state.xVel = initial.xVel + d.ddx*dt;
	state.yVel = initial.yVel + d.ddy*dt;
	
	//start prepping output
	Derivative output;
	
	//we already know the new velocity
	output.dx = state.xVel;
	output.dy = state.yVel;

	//calculate the new accelerations
	G_acceleration(state, output, t + dt, objMass, point_x, point_y);
	 
return output;
}

void integrate (Pixel_desc &state, float t, float dt, 
			const float objMass, const float point_x, const float point_y )
{
	Derivative init;
	init.dx = state.xVel;
	init.dy = state.yVel;

	init.ddx = state.xAcc;
	init.ddy = state.yAcc;

	Derivative a = evaluate( state, t, 0.0f, init, objMass, point_x, point_y);
	Derivative b = evaluate( state, t, dt* 0.5f, a, objMass, point_x, point_y);
	Derivative c = evaluate( state, t, dt*0.5f, b, objMass, point_x, point_y);
	Derivative d = evaluate( state, t, dt, c, objMass, point_x, point_y);

         float dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
         float dydt = 1.0f/6.0f * (a.dy + 2.0f*(b.dy + c.dy) + d.dy);


         float ddxdt = 1.0f/6.0f * (a.ddx + 2.0f*(b.ddx + c.ddx) + d.ddx);
         float ddydt = 1.0f/6.0f * (a.ddy + 2.0f*(b.ddy + c.ddy) + d.ddy);


	state.x = state.x + dxdt*dt;
	state.y = state.y + dydt*dt;

	state.xVel = state.xVel + ddxdt*dt;
	state.yVel = state.yVel + ddydt*dt;
}


//-------------------vvvv GENERALIZED FOR ENTITIES vvvvv------------

//need to decide how to apply all the forces to the object here
float G_acceleration_ent (const Entity_desc &state, Derivative &output, float t,
			const float objMass, const float objX, const float objY)
{
	const float gravConst = 10;

	float distance = (float)getDistance(objX, objY, state.x, state.y);
	float theta, gAccTotal=0.0;

	float x = (float)state.x;
	float y = (float)state.y;

	float deltaX = objX - x;
	float deltaY = objY - y;
		
		if(deltaY!=0 && deltaX!=0)	
			 theta = atan((deltaX)/(deltaY));
		else if(deltaY== 0 && deltaX!=0)
			 theta =3.1415926/2.0;
		else if(deltaY!= 0 && deltaX==0)
			 theta = 0.0;


		if((deltaX<0 && deltaY<0) || (deltaX>0 && deltaY<0))
			 {theta *=(-1.0);}

		if(distance!=0)// && distance>21 && distance < (objMass*2))
			 gAccTotal = (objMass*gravConst)/(distance*distance);
//std::cout<<"\t\t\tMass="<<objMass<<" gravConst="<<gravConst<<" distance="<<distance<<std::endl;
//std::cout<<"\t\t\t\tgAccelTotal = "<<gAccTotal<<std::endl;
			output.ddx = gAccTotal * sin(theta);
			output.ddy = gAccTotal * cos(theta);

			if(y>objY){output.ddy *=-1;}

	return 0.0;
}

Derivative evaluate_ent(Entity_desc &initial,float t, float dt,const Derivative &d,
				const float objMass, const float point_x, const float point_y)
{
	Entity_desc state;
//std::cout<<"\tIntitial State: "<<initial.x<<","<<initial.y<<" mass="<<initial.mass<<std::endl;	
//std::cout<<"\tDerivative: dx="<<d.dx<<" dy="<<d.dy<<" dt="<<dt<<std::endl;	
	//calc new positions
	state.x = initial.x + d.dx * dt; 
	state.y = initial.y + d.dy * dt; 

	//calc new velocities
	state.xVel = initial.xVel + d.ddx*dt;
	state.yVel = initial.yVel + d.ddy*dt;
	
	//start prepping output
	Derivative output;
	
	//we already know the new velocity
	output.dx = state.xVel;
	output.dy = state.yVel;

//std::cout<<"\tPass State: "<<state.x<<","<<state.y<<" mass="<<state.mass<<std::endl;	
	//calculate the new accelerations
	 G_acceleration_ent(state, output, t + dt, objMass, point_x, point_y);
	 

return output;
}
bool isNAN(float &num_to_check)
{
	if(num_to_check != num_to_check)
		return true;
	else
		return false;	
}
bool isDerivNAN(Derivative &derivCheck)
{
	if((isNAN(derivCheck.ddx) 
		|| isNAN(derivCheck.ddy) 
			|| isNAN(derivCheck.dx)
				|| isNAN(derivCheck.dy)))
	{
		std::cout<<"Nan found!"<<std::endl;
		return true;	
	}
	else
		return false;

}

void integrate_ent (Entity_desc &state, float t, float dt, 
			const float objMass, const float point_x, const float point_y )
{
	Derivative init;
	init.dx = state.xVel;
	init.dy = state.yVel;

	init.ddx = state.xAcc;
	init.ddy = state.yAcc;
//std::cout<<"Top Level: objMass="<<objMass<<" X="<<point_x<<" Y="<<point_y<<" Acting on "<<state.x<<","<<state.y<<"mass="<<state.mass<<std::endl;
	Derivative a, b, c, d;
	a=b=c=d=init;

	if(!isDerivNAN(init))	
		a = evaluate_ent( state, t, 0.0f, init, objMass, point_x, point_y);
	if(!isDerivNAN(a))
		b = evaluate_ent( state, t, dt* 0.5f, a, objMass, point_x, point_y);
	if(!isDerivNAN(b))
		c = evaluate_ent( state, t, dt*0.5f, b, objMass, point_x, point_y);
	if(!isDerivNAN(c))
		d = evaluate_ent( state, t, dt, c, objMass, point_x, point_y);

         float dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
         float dydt = 1.0f/6.0f * (a.dy + 2.0f*(b.dy + c.dy) + d.dy);


         float ddxdt = 1.0f/6.0f * (a.ddx + 2.0f*(b.ddx + c.ddx) + d.ddx);
         float ddydt = 1.0f/6.0f * (a.ddy + 2.0f*(b.ddy + c.ddy) + d.ddy);


	state.x = state.x + dxdt*dt;
	state.y = state.y + dydt*dt;

	state.xVel = state.xVel + ddxdt*dt;
	state.yVel = state.yVel + ddydt*dt;
}

#endif
