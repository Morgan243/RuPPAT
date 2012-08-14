#include "Game.h"
#include "unistd.h"

using namespace std;


//-----CONSTRUCTOR
Game :: Game()
{
	engine = new RuPPAT(1024, 768, BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);
	done = false;
}

//-----CONSTRUCTOR 2!!
Game :: Game(int WIDTH_cl, int HEIGHT_cl)
{
	engine = new RuPPAT(WIDTH_cl, HEIGHT_cl, BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);
	done = false;

	//players.push_back(new Player("red_ship.png",360, 0));	

	engine->addPlayer("red_ship.png", 360, 0, 1, 400, 200);
	
	//engine->addPlayer("white_rocket.png", 360, 0, 1, 400, 200);
	engine->addObject("red_planet.png", (int)(WIDTH_cl/2), (int)(HEIGHT_cl/2), 100000);
}


//-----DECONSTRUCTOR
Game ::~Game()
{
//	delete engine;
}

//some globals cuase im lazy
int x, y, up_count, down_count, left_count, right_count,
	 key_count_limit=0, defaultTurnAmnt=4, fastTurn=3,
		w_count=0, s_count=0, a_count=0, d_count=0;


bool k_UP=false, k_DOWN=false, k_LEFT=false, k_RIGHT=false,
	k_lCTRL=false, k_lMOUSE;


int mouse_x, mouse_y;
///////////////////////////////////////////////////
//|||||||||
//RUN the game!
void Game :: run(string selection, string option)
{
	//declare and init SDL events structure
	Event_desc mainEvents;
	initEvent(mainEvents );

	//defined in RuPPAT.h to pass stuff to engine
	runPPAT_helperArgs engineArgs;

	//assign engines options
	engineArgs.context = this;
	engineArgs.mDone =&done;
	engineArgs.mEvents = &mainEvents;
	engineArgs.selection = selection;
	engineArgs.option = option;


	void* args = &engineArgs;
	pthread_t RuPPAT_th;

	//Launch the RuPPAT engine its own thread
	pthread_create(&RuPPAT_th, NULL, runPPAT_helper,args); 

	int game_rate = 60;
	int interval = 1000/game_rate;
	int nextTick = SDL_GetTicks() + interval;

	//just keep checking events until done
	while(!done)
	{
		handleEvents(mainEvents);

		if(nextTick > SDL_GetTicks())SDL_Delay(nextTick - SDL_GetTicks());
	
	nextTick = SDL_GetTicks() + interval;	

	}
	
	//joining 
	cout<<"joining RuPPAT in GAME"<<endl;
	pthread_join(RuPPAT_th,NULL);

}


//This function is the only place that the mainEvent Event_desc can be written/altered
//all other threads only poll for values relevent to them
void Game :: handleEvents(Event_desc &mainEvents)
{
	while(SDL_PollEvent(&event))
	{
		//if evenet was quit, finsh everything up
		if(event.type == SDL_QUIT)
			done = true;


		//if a key was pressed
		if(event.type == SDL_KEYDOWN)
			{
			cout<<"keydown!"<<endl;


			if(event.key.keysym.sym == SDLK_LCTRL)
				{
					k_lCTRL = true;
				}

			if(event.key.keysym.sym == SDLK_ESCAPE)
				{
					done = true;
					cout<<"doneskies!!"<<endl;
				}

			if(event.key.keysym.sym == SDLK_SPACE)
				mainEvents.space = true;

			if(event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_k)
				{
						y-=2;
					k_UP=true;
				}
			if(event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_j)
				{
					y+=2;
					k_DOWN=true;
				}
			if(event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_h)
				{
					x-=2;
					k_LEFT=true;
				}
			if(event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_l)
				{				
					x+=2;
					k_RIGHT=true;
				}
			}
		//if a key was released
		if(event.type == SDL_KEYUP)
			{

			if(event.key.keysym.sym == SDLK_LCTRL )
			{
				k_lCTRL = false;
			}

			if(event.key.keysym.sym == SDLK_SPACE )
				mainEvents.space = false;

			if(event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_k)
				{
			
					k_UP=false;
				}
			if(event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_j)
				{
			
					k_DOWN=false;
				}
			if(event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_h)
				{
	
					k_LEFT=false;
				}
			if(event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_l)
				{				

					k_RIGHT=false;
				}
			}
	}


		//	HANDLE LONG PRESSES
		if(k_lCTRL)
			{
				//turn to position
			//	printf("would be turing towards: %d,%d",
			//	event.motion.x, event.motion.y);
					
				//mouse button one is pressed, store location of mouse in two ints	
				if(SDL_BUTTON(1)&SDL_GetMouseState(&mouse_x, &mouse_y))
				{
				engine->turnPlayerToCoord(0, mouse_x, mouse_y, defaultTurnAmnt);	
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
				engine->turnPlayerToCoord(0, mouse_x, mouse_y, defaultTurnAmnt);	
			}

		//handle counting the time of the mouse press
		//might need to change to make slower sysems more responsive 
		//(use ticks or someotherreal workld time to garuntee the "feel" of user IO)
		if(k_UP)
			{
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
}

//-------initEvent
//sets all the memebers of an Event_desc struct
//to initial/default values
void Game :: initEvent(Event_desc &initMe)
{
	initMe.space =false;
	initMe.w =false;
	initMe.a =false;
	initMe.s =false;
	initMe.d =false;
	initMe.up =false;
	initMe.left =false;
	initMe.down =false;
	initMe.right =false;
	initMe.enter =false;
	initMe.rightCtrl =false;
	initMe.leftCtrl =false;
}


void Game :: rotateSpriteToCoor(int p_ID,int x, int y, int rate)
{
//	printf("Curren ship angle: %f", getAngle());
	engine->turnPlayerToCoord( p_ID, x, y, rate);
}

