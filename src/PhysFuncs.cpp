#ifndef PHYS_FUNCS_H
#define PHYS_FUNCS_H

#include <math.h>
#include <iostream>
#include "PhysFuncs.h"
#include "Descriptors.h"
//#include "Object.h"


namespace PhysFunc
{
    float getDistance(float x1, float y1, float x2, float y2)
    {
        return ( sqrt( ((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1))  ) );
    }

    float G_force(const Entity_desc &state_1, const Entity_desc &state_2,
                    Derivative &output_1, Derivative &output_2)
    {
     //{{{
        const float gravConst = 10;

        float distance = (float)getDistance(state_2.x, state_2.y, state_1.x, state_1.y);
        float theta_1 = 0.0, theta_2 = 0.0, gForce=0.0;

        float deltaX = state_2.x - state_1.x;
        float deltaY = state_2.y - state_1.y;
            
            if(deltaY !=0 && deltaX != 0)	
                 theta_1 = atan((deltaX)/(deltaY));
            else if(deltaY == 0 && deltaX != 0)
                 theta_1 = 3.1415926/2.0;
            else if(deltaY != 0 && deltaX == 0)
                 theta_1 = 0.0;


            //adjust theta depending on the coordinate
            if((deltaX<0 && deltaY<0) || (deltaX>0 && deltaY<0))
                 theta_1 *=(-1.0);

            //theta_2 should be opposite theta 1, save some calculations
            if(theta_1 > 180)
                theta_2 = theta_1 - 180.0;
            else if (theta_1 < 180)
                theta_2 = theta_1 + 180;
            else
                theta_2 = 0;

            if(distance!=0 && distance>31)// && distance < (objMass*2))
                 gForce = (state_2.mass*state_1.mass*gravConst)/(distance*distance);

            //determine each objects acceleration based on the force and their masses
            output_1.ddx = (gForce * sin(theta_1))/state_1.mass;
            output_1.ddy = (gForce * cos(theta_1))/state_1.mass;

            output_2.ddx = (gForce * sin(theta_2))/state_2.mass;
            output_2.ddy = (gForce * cos(theta_2))/state_2.mass;

            //invers the y accel (caused by reversed y axis in SDL)
            if(state_1.y > state_2.y)
                output_1.ddy *=-1;
            else
                output_2.ddy *=-1;

        return gForce;
    //}}}
    }

    //need to decide how to apply all the forces to the object here
    float G_acceleration (const Pixel_desc &state, Derivative &output, float t,
                        const float objMass, const float objX, const float objY)
    {
    //{{{*
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
    //}}}
    }

    Derivative evaluate(Pixel_desc &initial,float t, float dt,const Derivative &d,
                        const float objMass, const float point_x, const float point_y)
    {
        //{{{
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
    //}}}
    }

    void integrate (Pixel_desc &state, float t, float dt, 
                        const float objMass, const float point_x, const float point_y )
    {
        //{{{
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
        //}}}
    }


    //-------------------vvvv GENERALIZED FOR ENTITIES vvvvv------------

    //need to decide how to apply all the forces to the object here
    float G_acceleration_ent (const Entity_desc &state, Derivative &output, float t,
                        const float objMass, const float objX, const float objY)
    {
        //{{{
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

            if(distance!=0 && distance>31)// && distance < (objMass*2))
                 gAccTotal = (objMass*gravConst)/(distance*distance);
                output.ddx = gAccTotal * sin(theta);
                output.ddy = gAccTotal * cos(theta);

                if(y>objY){output.ddy *=-1;}

        return 0.0;
        //}}}
    }

    Derivative evaluate_ent(Entity_desc &initial,float t, float dt,const Derivative &d,
                        const float objMass, const float point_x, const float point_y)
    {
        //{{{
        Entity_desc state;
        
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
         G_acceleration_ent(state, output, t + dt, objMass, point_x, point_y);
         

    return output;
    //}}}
    }


    void integrate_ent (Entity_desc &state, float t, float dt, 
                        const float objMass, const float point_x, const float point_y )
    {
        //{{{
        Derivative init;
        init.dx = state.xVel;
        init.dy = state.yVel;

        init.ddx = state.xAcc;
        init.ddy = state.yAcc;

        Derivative a, b, c, d;
        a=b=c=d=init;

            a = evaluate_ent( state, t, 0.0f, init, objMass, point_x, point_y);
            b = evaluate_ent( state, t, dt* 0.5f, a, objMass, point_x, point_y);
            c = evaluate_ent( state, t, dt*0.5f, b, objMass, point_x, point_y);
            d = evaluate_ent( state, t, dt, c, objMass, point_x, point_y);

             float dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
             float dydt = 1.0f/6.0f * (a.dy + 2.0f*(b.dy + c.dy) + d.dy);


             float ddxdt = 1.0f/6.0f * (a.ddx + 2.0f*(b.ddx + c.ddx) + d.ddx);
             float ddydt = 1.0f/6.0f * (a.ddy + 2.0f*(b.ddy + c.ddy) + d.ddy);


        state.x = state.x + dxdt*dt;
        state.y = state.y + dydt*dt;

        state.xVel = state.xVel + ddxdt*dt;
        state.yVel = state.yVel + ddydt*dt;
        //}}}
    }


    //-------------------vvvv GENERALIZED FOR OBJECTS vvvvv------------

    //need to decide how to apply all the forces to the object here
    //float G_acceleration(const Entity_desc &state, Derivative &output, float t,
    //			const Object &state_src)
    //			//const float objMass, const float objX, const float objY)
    //{
    //	//{{{
    //	const float gravConst = 10;
    //
    //	float distance = (float)getDistance(state_src.getX(), state_src.getY(), state.x, state.y);
    //	float theta, gAccTotal=0.0;
    //
    //	float x = (float)state.x;
    //	float y = (float)state.y;
    //
    //	float deltaX = state_src.getY() - x;
    //	float deltaY = state_src.getX() - y;
    //	
    //	cout<<"Destination: "<< x <<","<< y <<endl;
    //		cout<<"\tSource: "<< state_src.getX()<<","<< state_src.getY()<<endl;
    //
    //		if(deltaY!=0 && deltaX!=0)	
    //			 theta = atan((deltaX)/(deltaY));
    //		else if(deltaY== 0 && deltaX!=0)
    //			 theta =3.1415926/2.0;
    //		else if(deltaY!= 0 && deltaX==0)
    //			 theta = 0.0;
    //
    //
    //		if((deltaX<0 && deltaY<0) || (deltaX>0 && deltaY<0))
    //			 {theta *=(-1.0);}
    //
    //		if(distance!=0)// && distance>21 && distance < (objMass*2))
    //			 gAccTotal = (((float)state_src.getMass())*gravConst)/(distance*distance);
    //			output.ddx = gAccTotal * sin(theta);
    //			output.ddy = gAccTotal * cos(theta);
    //
    //			if(y>state_src.getY()){output.ddy *=-1;}
    //
    //			cout<<"\t G Accel Total = "<< gAccTotal<<endl;
    //			cout<<"\t\t X = "<<output.ddx<<" Y = "<<output.ddy<<endl;
    //	return 0.0;
    //	//}}}
    //}

    Derivative evaluate(Entity_desc &initial,float t, float dt,const Derivative &d,
                const Entity_desc &init_src)			
        //const float objMass, const float point_x, const float point_y)
    {
        //{{{
        Entity_desc state;// = initial.getDescriptor();
        
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
         G_acceleration_ent(state, output, t+dt, 
                 init_src.mass, init_src.x, init_src.y);

         
         

    return output;
    //}}}
    }


    void integrate (Entity_desc &state, float t, float dt, 
                Entity_desc &state_src)
    {
        //{{{
        Derivative init;
        init.dx = state.x;
        init.dy = state.y;

        init.ddx = state.xAcc;
        init.ddy = state.yAcc;

        Derivative a, b, c, d;
        a=b=c=d=init;

            a = evaluate( state, t, 0.0f, init, state_src);
            b = evaluate( state, t, dt* 0.5f, a, state_src);
            c = evaluate( state, t, dt*0.5f, b, state_src);
            d = evaluate( state, t, dt, c, state_src);

             float dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
             float dydt = 1.0f/6.0f * (a.dy + 2.0f*(b.dy + c.dy) + d.dy);


             float ddxdt = 1.0f/6.0f * (a.ddx + 2.0f*(b.ddx + c.ddx) + d.ddx);
             float ddydt = 1.0f/6.0f * (a.ddy + 2.0f*(b.ddy + c.ddy) + d.ddy);



        state.x = state.x + dxdt*dt;
        state.y = state.y + dydt*dt;

        state.xVel = state.xVel + ddxdt*dt;
        state.yVel = state.yVel + ddydt*dt;
        

        //state.setDescriptor(temp_ent);
        //}}}
    }
}
#endif
