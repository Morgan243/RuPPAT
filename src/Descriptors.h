#include <vector>
#include <iostream>
#include <string>
#include "SDL/SDL.h"
#ifndef __DESC_H
#define __DESC_H

#define LEFT 0
#define RIGHT 1
#define BACKWARD 2
#define FORWARD 3

using namespace std;

//basic conatiner for surface, group with name and ID number
//a more basic form of the Sprite class
struct Surface_Container
{
	SDL_Surface* surface;
	int ID, x, y;
	string name;
};

//floating point coordinates
struct CoOrd_fl
{
	float x,y;
};

//integer coordinates
struct CoOrd
{
	int x,y;
};

//individual option, EX: width
//with value EX: 800
struct optionSet
{
	std::string option;
	std::vector<std::string> values;
};

//individual section, block of options
//EX main or objects
struct section
{
	std::string title;
	std::vector<optionSet> sectionOptions;
};


//container for circle info
struct Circle_desc
{
	int x,y;
	unsigned int r;
};

//Base descriptor for all entities, or on screen moving things
struct Entity_desc
{
	float x;
	float y;

	float xVel, yVel;//velocity vector
	float xAcc, yAcc;//acceleration vector

	int mass;
	int ID;
	float maxAccel;

	int hitCircle_radius;
	bool noDrawEffect;
};

//easily pass an event container where it is needed
//pass by reference to use properly
struct Event_desc
{
	bool space;
	bool w,a,s,d;
	bool up,left,down,right;
	bool enter;
	bool rightCtrl, leftCtrl;
};

//A little different than entities; the idea being that these
//pixels get flung out into the game from a point and may fade
//out over time.
struct Pixel_desc : public Entity_desc
{
	//float x, y; //position
	Uint32 color;//color of pixel
	
	unsigned int dimTimer;

	int accelLength;//how long to apply acceleration
	
	int updated; //indicate if the pixel has been manipulated already

	bool updatedG;//hsa gravity accel been added?

	int dimFactor; //larger =equals quicker dimmming time

	bool deleteMe;
};

//container for accel and velocities
struct Derivative
{
	//velocities
	float dx;
	float dy;
	
	//accelerations
	float ddx;
	float ddy;
};
#endif
