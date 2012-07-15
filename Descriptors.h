#ifndef __DESC_H
#define __DESC_H

#define LEFT 0
#define RIGHT 1
#define BACKWARD 2
#define FORWARD 3

struct Entity_desc
{
	float x;
	float y;

	float xVel, yVel;//velocity vector
	float xAcc, yAcc;//acceleration vector
	int xGacc,yGacc; //grav acceleration
	int mass;
//	int spriteID; //base sprite (missle, ship, etc)
//	int rotationID; //which rotation version of base

	float maxAccel;
	bool noDrawEffect;

};

struct Event_desc
{
	bool space;
	bool w,a,s,d;
	bool up,left,down,right;
	bool enter;
	bool rightCtrl, leftCtrl;


};

struct Pixel_desc
{
	int ID;//identify the pixel, usefule for debugging

	float x, y; //position
	unsigned int color;//color of pixel
	
	int mass; //mass	

	float xVel, yVel;//velocity vector

	float xAcc, yAcc;//acceleration vector
	int xGacc,yGacc; //grav acceleration

	unsigned int Xtimer;//used for time based movement
	unsigned int Ytimer;	
	unsigned int dimTimer;

	int accelLength;//how long to apply acceleration
	
	int updated; //indicate if the pixel has been manipulated already

	bool updatedG;//hsa gravity accel been added?

	int dimFactor; //larger =equals quicker dimmming time

	bool deleteMe;
};

struct Mass_desc
{
	int mass;

	int xLoc;
	int yLoc;

//	SDL_Surface *mass_sprite;

};


struct Grav_desc
{	
	int x, y;
	
	int gXacc;
	int gYacc;

};

#endif
