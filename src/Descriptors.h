#include <vector>
#include <iostream>
#include "SDL/SDL.h"
#ifndef __DESC_H
#define __DESC_H

#define LEFT 0
#define RIGHT 1
#define BACKWARD 2
#define FORWARD 3


struct RunOptions
{
	int width;
	int height;
	std::string selection;	
	std::string option;

	std::string background_spritePaths[3];

	std::string player_spritePath;
	std::string player_HCpath;

	std::vector <std::string> objects_spritePath;
	std::vector <std::string> objects_HCpath;
};

struct Circle_desc
{
	int x,y;
	unsigned int r;
};

struct Entity_desc
{
	float x;
	float y;

	float xVel, yVel;//velocity vector
	float xAcc, yAcc;//acceleration vector
//	int xGacc,yGacc; //grav acceleration
	int mass;
	int ID;
	float maxAccel;
	bool noDrawEffect;
	
	std::vector<Circle_desc> hitCircles;
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
	Uint32 color;//color of pixel
	
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

#endif
