#include "Game.h"
#include "unistd.h"
#include <thread>
using namespace std;


//-----CONSTRUCTOR: configSections holds vector of config sections
// 			   sections hold vectors of options, which have vectors of values 
Game::Game(vector<section> configSections)
{
//{{{

	//A few defaults, not configurable at the moment
	key_count_limit = 2;
	defaultTurnAmnt = 4;
	fastTurn = 3;
	w_count = 0;
	s_count = 0;
	a_count = 0;
	d_count = 0;

	k_UP=false; k_DOWN=false; k_LEFT=false;
       	k_RIGHT=false;k_lCTRL=false;

	vector<string> tempVect;	
	vector<string> tempVect_2;
	string missile_sprite;
	
	for(int i = 0; i< configSections.size(); i++)
	{
		//////////////////////////////////////////////////
		//MAIN section: resolution and background settings
		//////////////////////////////////////////////////	
		if(configSections[i].title == "Main")
		{

			int width, height;
			for(int j = 0; j < configSections[i].sectionOptions.size();j++)
			{
				if(configSections[i].sectionOptions[j].option 
						== "width")
					width =
					atoi(configSections[i].sectionOptions[j].values[0].c_str());

				else if(configSections[i].sectionOptions[j].option
					       	== "height")
				        height = 
					atoi(configSections[i].sectionOptions[j].values[0].c_str());

				else if(configSections[i].sectionOptions[j].option
						== "backgroundLayers")
					tempVect = configSections[i].sectionOptions[j].values;
			}

			engine = new RuPPAT(width, height, BPP,
					    SDL_HWSURFACE | SDL_DOUBLEBUF,
					    tempVect);
		}

		//Setup player: object controlled by the player(person-user)
		if(configSections[i].title == "Player")
		{
			int x = 400, y = 200, startingAngle = 0, numRots = 360;
			float maxAcc = 1.0;

			for(int j = 0; j < configSections[i].sectionOptions.size();j++)
			{
				if(configSections[i].sectionOptions[j].option
						== "sprite")
					tempVect = configSections[i].sectionOptions[j].values;

				else if(configSections[i].sectionOptions[j].option
						== "HC")
					tempVect_2 = configSections[i].sectionOptions[j].values;

				else if(configSections[i].sectionOptions[j].option
						== "x")
					x = 
					atoi(configSections[i].sectionOptions[j].values[0].c_str());

				else if(configSections[i].sectionOptions[j].option
						== "y")
					y = 
					atoi(configSections[i].sectionOptions[j].values[0].c_str());

				else if(configSections[i].sectionOptions[j].option
						== "angle")
					startingAngle = 
					atoi(configSections[i].sectionOptions[j].values[0].c_str());

				else if(configSections[i].sectionOptions[j].option
						== "maxAcc")
					maxAcc = 
					atof(configSections[i].sectionOptions[j].values[0].c_str());
				
				else if(configSections[i].sectionOptions[j].option
						== "missle")
				 missile_sprite = configSections[i].sectionOptions[j].values[0];

			}

	//create the player
	Player *tempPlayer = new Player(tempVect[0], numRots, 
			startingAngle, maxAcc,x, y, tempVect_2[0]);
		
		//give player a missile
		tempPlayer->addMissile(missile_sprite, "Orange Blob", true,
					100, 10);//amount, max damage
		
		engine->addPlayer(tempPlayer);
		  
		}

		//Setup andy additional object requested
		if(configSections[i].title == "Object")
		{
			int x = 400, y = 200, startingAngle = 0, numRots = 360,
				mass = 750;

			float maxAcc = 1.0, rotRate = 0.0, xVel = 0.0, yVel = 0.0;

			for(int j = 0; j < configSections[i].sectionOptions.size();j++)
			{
				if(configSections[i].sectionOptions[j].option
						== "spriteBase")
					tempVect = 
						configSections[i].sectionOptions[j].values;

				else if(configSections[i].sectionOptions[j].option
						== "HC")
					tempVect_2 = 
						configSections[i].sectionOptions[j].values;

				else if(configSections[i].sectionOptions[j].option
						== "x")
					x = 
					atoi(configSections[i].sectionOptions[j].values[0].c_str());

				else if(configSections[i].sectionOptions[j].option
						== "y")
					y = 
					atoi(configSections[i].sectionOptions[j].values[0].c_str());

				else if(configSections[i].sectionOptions[j].option
						== "angle")
					startingAngle = 
					atoi(configSections[i].sectionOptions[j].values[0].c_str());

				else if(configSections[i].sectionOptions[j].option
						== "maxAcc")
					maxAcc = 
					atof(configSections[i].sectionOptions[j].values[0].c_str());

				else if(configSections[i].sectionOptions[j].option
						== "rotation")
					rotRate = 
					atof(configSections[i].sectionOptions[j].values[0].c_str());

				else if(configSections[i].sectionOptions[j].option
						== "mass")
					 mass = 
					atoi(configSections[i].sectionOptions[j].values[0].c_str());
					
				else if(configSections[i].sectionOptions[j].option
						== "xVel")
					 xVel = 
					atof(configSections[i].sectionOptions[j].values[0].c_str());

				else if(configSections[i].sectionOptions[j].option
						== "yVel")
					yVel = 
					atof(configSections[i].sectionOptions[j].values[0].c_str());
			}

			engine->addObject(tempVect[0],
				      	  x, y, mass,
				  	  rotRate,
					  xVel, yVel,
					  tempVect_2[0]);	  
		}
	}
//}}}
}

//-----DECONSTRUCTOR
Game ::~Game()
{
//	delete engine;
}

//keep track of button presses
bool k_UP=false, k_DOWN=false, k_LEFT=false, k_RIGHT=false,
	k_lCTRL=false, k_lMOUSE;

//mouse position
int mouse_x, mouse_y;

///////////////////////////////////////////////////
//|||||||||
//RUN the game!
void Game :: run(string selection, string option)
{
//{{{
	//declare and init SDL events structure
	Event_desc mainEvents;
	initEvent( mainEvents );

	//Launch the particle physics and time engine!
	std::thread ruppat_th(&RuPPAT::runPPAT, engine,
			&done, &mainEvents, selection, option);


	int game_rate = 120;
	int interval = 1000/game_rate;
	
	//get the tick# when events should be handled
	int nextTick = SDL_GetTicks() + interval;

	//just keep checking events until done
	while(!done)
	{
		handleEvents(mainEvents);

		//should the game wait to check events	
		if(nextTick > SDL_GetTicks())
			SDL_Delay(nextTick - SDL_GetTicks());
	
		nextTick = SDL_GetTicks() + interval;	

	}
	
	//joining 
	cout<<"joining RuPPAT in GAME"<<endl;

	ruppat_th.join();
//}}}
}


//This function is the only place that the mainEvent Event_desc
//can be written/altered all other threads only poll for values 
//relevent to them
void Game :: handleEvents(Event_desc &mainEvents)
{
//{{{
	//keep working while events exist
	while(SDL_PollEvent(&event))
	{
		//if evenet was quit, finsh everything up
		if(event.type == SDL_QUIT)
			done = true;


		//if a key was pressed
		if(event.type == SDL_KEYDOWN)
			{
			//cout<<"keydown!"<<endl;


			//LEFT CONTROL
			if(event.key.keysym.sym == SDLK_LCTRL)
				{
					k_lCTRL = true;
				}

			//ESCAPE
			if(event.key.keysym.sym == SDLK_ESCAPE)
				{
					done = true;
					cout<<"doneskies!!"<<endl;
				}

			//SPACE
			if(event.key.keysym.sym == SDLK_SPACE)
				mainEvents.space = true;

			//UP, W, K
			if(event.key.keysym.sym == SDLK_UP 
				|| event.key.keysym.sym == SDLK_w 
				|| event.key.keysym.sym == SDLK_k)
				{
					y-=2;
					k_UP=true;
				}

			//DOWN,S,J
			if(event.key.keysym.sym == SDLK_DOWN 
				|| event.key.keysym.sym == SDLK_s 
				|| event.key.keysym.sym == SDLK_j)
				{
					y+=2;
					k_DOWN=true;
				}

			//LEFT,A,H
			if(event.key.keysym.sym == SDLK_LEFT 
				|| event.key.keysym.sym == SDLK_a 
				|| event.key.keysym.sym == SDLK_h)
				{
					x-=2;
					k_LEFT=true;
				}

			//RIGHT,D,L
			if(event.key.keysym.sym == SDLK_RIGHT 
				|| event.key.keysym.sym == SDLK_d 
				|| event.key.keysym.sym == SDLK_l)
				{				
					x+=2;
					k_RIGHT=true;
				}
			}
		//if a key was RELEASED
		if(event.type == SDL_KEYUP)
			{

			//LEFT CONTROL
			if(event.key.keysym.sym == SDLK_LCTRL )
			{
				k_lCTRL = false;
			}

			//SPACE
			if(event.key.keysym.sym == SDLK_SPACE )
				mainEvents.space = false;

			//UP, W,K
			if(event.key.keysym.sym == SDLK_UP
				|| event.key.keysym.sym == SDLK_w 
				|| event.key.keysym.sym == SDLK_k)
				{
			
					k_UP=false;
				}

			//DOWN, S, J
			if(event.key.keysym.sym == SDLK_DOWN 
				|| event.key.keysym.sym == SDLK_s 
				|| event.key.keysym.sym == SDLK_j)
				{
			
					k_DOWN=false;
				}

			//LEFT,A,H
			if(event.key.keysym.sym == SDLK_LEFT 
				|| event.key.keysym.sym == SDLK_a 
				|| event.key.keysym.sym == SDLK_h)
				{
	
					k_LEFT=false;
				}

			//RIGHT,D,L
			if(event.key.keysym.sym == SDLK_RIGHT 
				|| event.key.keysym.sym == SDLK_d 
				|| event.key.keysym.sym == SDLK_l)
				{				

					k_RIGHT=false;
				}
			}
	}


		//HANDLE LONG PRESSES
		if(k_lCTRL)
			{
			//mouse button one is pressed, store location of mouse 
			//in two ints
			if(SDL_BUTTON(1)&SDL_GetMouseState(&mouse_x, &mouse_y))
				{
				engine->turnPlayerToCoord(0, mouse_x, mouse_y,
								 defaultTurnAmnt);

					//check if its time to accelerate
					if(up_count>key_count_limit)
						{
						engine->accelPlayer(0,true);
						up_count=0;
						}
					else
						{
						up_count++;
						}
				}
			else
				//otherwise, left click isn't pressed, turn the player
				engine->turnPlayerToCoord(0, mouse_x, mouse_y,
								 defaultTurnAmnt);

			}

		//handle counting the time of the key presses
		//might need to change to make slower sysems more responsive 
		//(use ticks or someotherreal world time to garuntee the "feel" of user IO)
		if(k_UP)
			{
			//reached desired time between accels, so accel and reset counter
			if(up_count>key_count_limit)
				{
				engine->accelPlayer(0,true);
				up_count=0;
				}
			else//otherwise, just increment the count
				{
				up_count++;
				}			
			}

		if(k_DOWN)
			{
			if(down_count>key_count_limit)		
				{
				engine->accelPlayer(0,false);
				down_count=0;
				}
			else
				{
				down_count++;
				}
			}

		if(k_LEFT)
			{
			if(left_count>key_count_limit)
				{
				engine->turnPlayer(0,true, defaultTurnAmnt);
				left_count=0;
				}
			else
				{
				left_count++;
				}
			}

		if(k_RIGHT)
			{
			if(right_count>key_count_limit)
				{
				engine->turnPlayer(0,false,defaultTurnAmnt );
				right_count=0;
				}
			else
				{
				right_count++;
				}
			}
//}}}
}

//-------initEvent
//sets all the memebers of an Event_desc struct
//to initial/default values
void Game :: initEvent(Event_desc &initMe)
{
//{{{
	initMe.space = false;
	initMe.w = false;
	initMe.a = false;
	initMe.s = false;
	initMe.d = false;
	initMe.up = false;
	initMe.left = false;
	initMe.down = false;
	initMe.right = false;
	initMe.enter = false;
	initMe.rightCtrl = false;
	initMe.leftCtrl = false;
//}}}
}

//interface provided object containing Game
void Game :: rotateSpriteToCoor(int p_ID, int x, int y, int rate)
{
	engine->turnPlayerToCoord( p_ID, x, y, rate);
}

