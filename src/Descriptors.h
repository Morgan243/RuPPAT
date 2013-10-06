// 				Descriptors
//--------------------------------------------------------------------
//| 	A set of definitions of "Descriptors": structs used as simple
//|data containers, intended to make data passing efficient and easy
//--------------------------------------------------------------------
#include <vector>
#include <iostream>
#include <string>
#include "SDL2/SDL.h"
#ifndef __DESC_H
#define __DESC_H

#define LEFT 0
#define RIGHT 1
#define BACKWARD 2
#define FORWARD 3

using namespace std;
enum JOB_ID {FIRE_MISSILE, COPY_OBJECT, COPY_SPRITE};
enum LEVEL_STYLE {WRAP};

//basic conatiner for surface, group with name and ID number
//a more basic form of the Sprite class
struct Surface_Container
{
	SDL_Surface* surface;
	int ID, x, y;
	string name;
};

struct Texture_Container
{
    SDL_Texture *texture;
	int ID, x, y, h, w;
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

struct CollisionCircleSlices_Container
{
    int num_slices;
    float deg_per_slice;
    vector<int> slice_radius;
};

//Base descriptor for all entities, or on screen moving things
struct Entity_desc
{
	int ID;

	float x;
	float y;

	int mass;

	float xVel, yVel;//velocity vector
	float xAcc, yAcc;//acceleration vector
    float xForce, yForce;

	float maxAccel;
    float maxForce;

	int hitCircle_radius;
    CollisionCircleSlices_Container hit_slices;
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

struct Renderables_Cont
{
	vector<Surface_Container> sprites;
	vector<Texture_Container> sprites_text;
	vector<Pixel_desc> pixels;
	vector<Entity_desc*> entities;
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

template <typename T>
struct Function_Container
{
    //object to call it on
    T *context;

    void (T::*function)() = NULL;
    //pointer to a function that accepts a void* and returns a void*
    void* (T::*arg_function)(void* args) = NULL;
    
    //argument to pass to the above function pointer
    void* argument;

    //whether the job executer should rgive the return item back
    bool fetch_return;
};

struct RenderJob_Container
{
    JOB_ID id;
    void* opt_container = NULL;
    void* return_obj = NULL;
};


struct RuPPAT_Options
{
    int width, height, bpp;
    unsigned int flags;
    
    vector<string> background_paths;
    LEVEL_STYLE style;
};

struct RunOptions
{
    string config_path;
};

#endif
