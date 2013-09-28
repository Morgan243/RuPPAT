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

    //-------------------vvvv Forced Based Methodology vvvvv------------
    float G_force(Entity_desc &state_1, Entity_desc &state_2)
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
            if(state_1.mass > 0)
            {
                state_1.xForce += (gForce * sin(theta_1));

                if(state_1.y > state_2.y)
                    state_1.yForce -= (gForce * cos(theta_1));
                else
                    state_1.yForce += (gForce * cos(theta_1));
            }
            else
            {
                state_1.xForce = 0;
                state_1.yForce = 0;
            }

            if(state_2.mass > 0)
            {
                state_2.xForce += (gForce * sin(theta_2));

                if(state_2.y > state_1.y)
                    state_2.yForce -= (gForce * cos(theta_2));
                else
                    state_2.yForce += (gForce * cos(theta_2));
            }
            else
            {
                state_2.xForce = 0;
                state_2.yForce = 0;
            }

            //invers the y accel (caused by reversed y axis in SDL)
//            if(state_1.y > state_2.y)
//                output_1.ddy *=-1;
//            else
//                output_2.ddy *=-1;

        return gForce;
    //}}}
    }

    void evaluate_force(Entity_desc &initial,float t, float dt,
                         Derivative &output_1)
    {
    //{{{
        
        //calc new positions
        initial.x = initial.x + output_1.dx * dt; 
        initial.y = initial.y + output_1.dy * dt; 


        //calc new velocities
        initial.xVel = initial.xVel + output_1.ddx*dt;
        initial.yVel = initial.yVel + output_1.ddy*dt;

        
        //we already know the new velocity
        output_1.dx = initial.xVel;
        output_1.dy = initial.yVel;

        output_1.ddx = initial.xForce / initial.mass;
        output_1.ddy = initial.yForce / initial.mass;

        //calculate new forces
        //G_force(initial, init_src, output_1, output_2);

    //}}}
    }

    void integrate_force (Entity_desc &state, float t, float dt)
    {
    //{{{
        //cout<<"---->state mass = " << state.mass<<endl;
        Derivative init;
            init.dx = state.xVel;
            init.dy = state.yVel;

            init.ddx = state.xAcc;
            init.ddy = state.yAcc;


        Derivative a_1, b_1, c_1, d_1, a_2, b_2, c_2, d_2;
            a_1=b_1=c_1=d_1=init;
            
        
        evaluate_force( state, t, 0.0f, init );
        evaluate_force( state, t, dt* 0.5f, a_1);
        evaluate_force( state, t, dt*0.5f, b_1);
        evaluate_force( state, t, dt, c_1);

             float dxdt_1 = 1.0f/6.0f * (a_1.dx + 2.0f*(b_1.dx + c_1.dx) + d_1.dx);
             float dydt_1 = 1.0f/6.0f * (a_1.dy + 2.0f*(b_1.dy + c_1.dy) + d_1.dy);

             float ddxdt_1 = 1.0f/6.0f * (a_1.ddx + 2.0f*(b_1.ddx + c_1.ddx) + d_1.ddx);
             float ddydt_1 = 1.0f/6.0f * (a_1.ddy + 2.0f*(b_1.ddy + c_1.ddy) + d_1.ddy);

        state.x = state.x + dxdt_1*dt;
        state.y = state.y + dydt_1*dt;

        state.xVel = state.xVel + ddxdt_1*dt;
        state.yVel = state.yVel + ddydt_1*dt;

//        cout<<"dxdt_1 = "<< dxdt_1<<endl;
//        cout<<"dxdt_1 = "<< dxdt_1<<endl;
//
//        cout<<"ddxdt_1 = "<< ddxdt_1<<endl;
//        cout<<"ddxdt_1 = "<< ddxdt_1<<endl;
//
//        cout<<"xForce = " <<state.xForce<<endl;
//        cout<<"yForce = " <<state.yForce<<endl;
//        cout<<"xAcc = " <<state.xAcc<<endl;
//        cout<<"yAcc = " <<state.yAcc<<endl;
//        cout<<"xVel = " <<state.xVel<<endl;
//        cout<<"yVel = " <<state.yVel<<endl;
//        cout<<"x = " <<state.x<<endl;
//        cout<<"y = " <<state.y<<endl;
//
        state.xForce = state.yForce = 0;
        state.xAcc = state.yAcc = 0;

    //}}}
    }
}
#endif
