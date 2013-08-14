/*	Rudimentary Particle, Physics, and Time Engine
 *			>>>RuPPAT<<<
 *---As it implies, this object contains functions to not only calculate
 * the physics of sprites and pixels in a given time frame, but also render
 * them properly 
*/

#include "RuPPAT.h"
#include "Object.h"
#include "Common.h"
#include <pthread.h>
#include <thread>
#include <mutex>
#include <omp.h>
#include <unistd.h>


using namespace std;


//------------CONSTRUCTOR--------
RuPPAT :: RuPPAT(int width,
	   			 int height,
				 int bpp,
				 unsigned int flags,
		 	     vector<string> bkg_paths)
{
//{{{
    
    initLocks();

	//start up the renderer
	mainRender = new Render(width, height, BPP, flags);

    //load the background sprites
	for( int i = 0; i< bkg_paths.size(); i++)
	{
		SDL_Surface *tempBkg, * tempBkgOpt;
		
		cout<<"Loading..."<<bkg_paths[i]<<endl;

		tempBkg = IMG_Load((char *)bkg_paths[i].c_str());
	
		cout<<"\tReassigning..."<<bkg_paths[i]<<endl;
		tempBkgOpt = SDL_DisplayFormatAlpha(tempBkg);
		
		SDL_SetAlpha(tempBkgOpt, SDL_SRCALPHA, 0xFF);

		SDL_FreeSurface(tempBkg);

		cout<<"\tPushing back..."<<bkg_paths[i]<<endl;
		backgroundLayers.push_back(tempBkgOpt);	
	}
    
    //base the game boundries based on the height and width
	game_width = backgroundLayers[0]->w;
	game_height = backgroundLayers[0]->h;

    //make sure common has the width and height values
	Common::SetDimensions(game_width, game_height);

    //let render know the boundries
	mainRender->setGameArea(backgroundLayers[0]->w, backgroundLayers[0]->h);

	printf("game width = %d, game height = %d\n", game_width, game_height);

	WIDTH = width;
	HEIGHT = height;

    //primitive factory
	primitive_maker = new Primitives(1,
		   							0x55efe000,
								   	mainRender->pre_surface);

    //hard coded font file, need to get this from the config
    //however, not bothering to due to a planned structure overhaul
    this->text_driver = new TextDriver("main", "FontSheet.png",
                                        25, 25, 12, 12); //width, height, rows, columns

	gravitationalConstant = 22;
//}}}
}


//------------DECONSTRUCTOR-------
//destroy the mutexes
RuPPAT :: ~RuPPAT()
{
//{{{
	//destroy mutexes
	pthread_mutex_destroy(&pix_list_lock_2);
	pthread_rwlock_destroy(&pix_rw_lock);
	pthread_rwlock_destroy(&mass_rw_lock);
	pthread_rwlock_destroy(&ent_rw_lock);
	pthread_rwlock_destroy(&player_rw_lock);
	pthread_rwlock_destroy(&object_rw_lock);
//}}}
}


void RuPPAT::initLocks()
{
//{{{
	//init all the locks with error check
	if(pthread_rwlock_init(&pix_rw_lock, NULL))
		{cout<<"ERROR initializing pixel rw lock: "<<endl;}

	if(pthread_rwlock_init(&mass_rw_lock, NULL))
		{cout<<"ERROR initializing mass rw lock: "<<endl;}

	if(pthread_rwlock_init(&ent_rw_lock, NULL))
		{cout<<"ERROR initializing entity rw lock: "<<endl;}

	if(pthread_rwlock_init(&player_rw_lock, NULL))
		{cout<<"ERROR initializing player rw lock: "<<endl;}

	if(pthread_mutex_init(&pix_list_lock_2,NULL))
		{cout<<"ERROR initializing pix lock 2 lock: "<<endl;}
	
	if(pthread_rwlock_init(&object_rw_lock,NULL))
		{cout<<"ERROR initializing object rw lock: "<<endl;}
//}}}
}


int RuPPAT :: addObject(const string spritePath,
	   					const int x,
					   	const int y,
					   	const int mass,
					   	const float rotationRate,
						const float xVel,
					   	const float yVel,
					   	const string HB_path)
{
//{{{
	pthread_rwlock_wrlock(&object_rw_lock);

		if(HB_path == "")
		{
            objectList.push_back(new Object(spritePath, x, y, mass,
                            360,0,xVel,yVel, HB_path));
            cout<<spritePath<<" doesnt have HB!"<<endl;
		}			
		else
		{
            objectList.push_back(new Object(spritePath, x, y, mass,
                            360,0,xVel,yVel, HB_path ));
        
            cout<<spritePath<<" DOES have HB!"<<endl;
		}

		objectList.back()->setID(baseID++);
		objectList.back()->sprite.setRotationRate(rotationRate);

	pthread_rwlock_unlock(&object_rw_lock);

	return objectList.size();
//}}}
}

void RuPPAT :: addPlayer(Player* new_player)
{
//{{{
	int size, height, width;
	CoOrd tempCoOrd;

    Object *new_object = &(*new_player);
        new_object->setID(baseID++);


    pthread_rwlock_wrlock(&object_rw_lock);
        players.push_back(new_player);
        size = players.size()-1;
    
        objectList.push_back(new_object);

        centerX = new_object->getX();
        centerY = new_object->getY();

        new_object->sprite.getDimensions(width, height);

        primitive_maker->drawCircle(tempCoOrd,15 , true);
    pthread_rwlock_unlock(&object_rw_lock);
//}}}
}

//------------addPlayer----------------
//Using arguments, create a new player
//object and add it to players and add
//its base object class to objects
int RuPPAT :: addPlayer(const string spritePath,
	   					const int numRotations,
					   	const int startingAngle,
						const float maxAccel,
					   	const int x,
					   	const int y,
					   	const string HC_path)
{
//{{{
    int size = 0;
	CoOrd tempCoOrd;

    //create the new player
    Player *new_player= new Player(spritePath, 
                                    numRotations,
                                    startingAngle,
						            maxAccel, 
                                    x, y, 
                                    HC_path);

    //extract the base class
    Object *new_object = &(*new_player);
        new_object->setID(baseID++);

    pthread_rwlock_wrlock(&object_rw_lock);
        players.push_back(new_player);

        size = players.size()-1;
    
        primitive_maker->drawCircle(tempCoOrd, 19 , true);

        objectList.push_back(new_object);
    pthread_rwlock_unlock(&object_rw_lock);

	return size;
//}}}
}



//||||||||||||||||||||||||||||||||||||\\
//-------------------------------------\\
//	RuPPAT Entry Point
//spawn threads to do work and handle events
//
void RuPPAT :: runPPAT(const bool *mainDone,
	   				   const Event_desc *mainEvents,
					   const string selection,
					   const string option)

{
//{{{
	bool *DONE = (bool*)mainDone;

	cout<<"SELECTION: "<<selection<<"\t OPTION: "<<option<<endl;

	char select;
	
	//launch thread into rk4 parse
	std::thread RK4_parse_th(&RuPPAT::RK4_parse, this);
	
	void *sel = &select;

	int FPS=1000;
	unsigned int keyT1, keyT2;

	int game_rate = 120;
	int interval = 1000/game_rate;
	int nextTick = SDL_GetTicks() + interval;
	
	//keep going until game thread says DONE
	while(!*DONE)
	{
		//if space is pressed and it has been
		//a while since it was last pressed
		if(mainEvents->space && keyT1>20)
			{

			//reset the key tick count
			keyT1 = 0;

			//then run demo->place pixels w/ attr
			//runDemos(sel);
			firePlayersWeapon(0);
			}

		//increment keypress counter
		keyT1++;

		//sleep thread, keeps CPU usage down
		if(nextTick > SDL_GetTicks())
			SDL_Delay(nextTick - SDL_GetTicks());
			
		nextTick = SDL_GetTicks() + interval;	

	}

	//tell threads to finish
	done = true;

	//join on RK4_parse thread
	RK4_parse_th.join();
//}}}
}

//-------------RK4--------------
// 	A new implementation of the
// RK4_all functionality. By using object
// slicing, the Runge Kutta calculations
// can be made more generic 
void RuPPAT::RK4(const float t, const float dt)
{
//{{{
	//some temporary variables
	float new_x=111, new_y=111, new_xVel=0, new_yVel=0;

	//how many Objects are there to deal with
	pthread_rwlock_rdlock(&object_rw_lock);
	    int num_objects = objectList.size();
	pthread_rwlock_unlock(&object_rw_lock);

	//how many Pixels are there to deal with
	pthread_rwlock_rdlock(&pix_rw_lock);
        int num_pixels = pixelList_m.size();
	pthread_rwlock_unlock(&pix_rw_lock);

	Renderables_Cont* renderables;
	vector<Entity_desc*> *tempAuxDesc_ref;
	Entity_desc *obj_desc_prim , *obj_desc_sec, obj_desc_tri;
	Pixel_desc pix_desc;

 //go through object list, apply gravity 
 //to other objects and pixels
 for(int i = 0; i<num_objects;i++)
 {

	//go through the object list to apply grav to other objects
	for(int j = 0; j<num_objects; j++)
	{
	  //make sure not to apply the grav to objects self
	  if(i!=j)
	  {
		//Runge Kutta integrator
		pthread_rwlock_wrlock(&object_rw_lock);	

			//Get the outer loop objects descriptor
			obj_desc_tri = objectList[j]->getDescriptor();
		
			//Apply outer loop object [i] to the current object
			obj_desc_prim = objectList[i]->PhysicsHandler(t, dt, obj_desc_tri);

			if((renderables = objectList[i]->GetRenderables()) != NULL)
			{
				for(int i=0; i<renderables->entities.size(); i++)
				{
					Common::TestBounds(*renderables->entities[i], true);
				}
			}

			//check new locations are valid, reset them if not 
			Common::TestBounds(*obj_desc_prim, true);

		pthread_rwlock_unlock(&object_rw_lock);
	  }
	}
	//next, go through the pixel structs 
	for(int j = 0; j<num_pixels; j++)
	{

		pthread_rwlock_rdlock(&object_rw_lock);
		 pthread_rwlock_wrlock(&pix_rw_lock);

			obj_desc_sec = &pixelList_m[j];
			obj_desc_sec = objectList[i]->PhysicsHandler(*obj_desc_sec, t, dt);

			new_x = obj_desc_sec->x;
			new_y = obj_desc_sec->y;

			new_xVel = obj_desc_sec->xVel;
			new_yVel = obj_desc_sec->yVel;

		 pthread_rwlock_unlock(&pix_rw_lock);	
		pthread_rwlock_unlock(&object_rw_lock);

			//make sure the new location is within bounds
        Common::TestBounds(new_x, new_y, new_xVel, new_yVel, true);

		pthread_rwlock_wrlock(&pix_rw_lock);
			obj_desc_sec->xVel = new_xVel;
			obj_desc_sec->yVel = new_yVel;	
			obj_desc_sec->x = new_x;
			obj_desc_sec->y = new_y;
		
            //if the dim factor is not zero, apply it
			if(pixelList_m[j].dimFactor)
				Common::ApplyDimming(pixelList_m[j]);

			if(pixelList_m[j].deleteMe)
				{
					handleDelete(j);
					num_pixels = pixelList_m.size();
				}
		pthread_rwlock_unlock(&pix_rw_lock);
	}
 }
//}}}
}

//-------------RK4_parse-----------
//keeps running RK4_all (determines movement
//based on acceleration and such on all
//elements in pixel list), incrementing time 
//and rendering the pixels
void RuPPAT :: RK4_parse()
{
//{{{
    Uint32 T1=0, T2=1000;
    T1=SDL_GetTicks(); 
    float t = 0.0;
    float dt = 0.004;

    int engine_rate = 160;
    int interval = 1000/engine_rate;
    int nextTick = SDL_GetTicks() + interval;
    int  screen_centX=WIDTH/2, screen_centY=HEIGHT/2;

    char sel = 'g';
    void * select = &sel;

    SDL_Surface *primitives =
                SDL_CreateRGBSurface(SDL_SWSURFACE,600,600,32, 0xFF000000, 
                0x00FF0000, 0x0000FF00, 0x000000FF); 

    Primitives primMaker(1,0x9f0000ff, mainRender->pre_surface);
    CoOrd pA, pB, pC, pD, pCenter;
    pCenter.x = 200;
    pCenter.y = 200;


    pA.x = 200;
    pA.y = 400;

    pB.x = 200;
    pB.y = 100;

    pC.x = 100;
    pC.y = 200;

    pD.x = 100;
    pD.y = 200;

    Surface_Container test_text = text_driver->createTextBox("main", "test", "abcde");

    std::thread *rk4_th = new std::thread(&RuPPAT::RK4,this,t,dt);

	//keep looping until program end
	while(!done)
	  {
		//increment time
		t += dt;
	
		//blit bottom layer	
		mainRender->applySurface(xOrigin/1.2,yOrigin/1.2,backgroundLayers[0]);
		
		//blit middle layer
		mainRender->applySurface(xOrigin,yOrigin,backgroundLayers[1]);

		
		SDL_SetAlpha(mainRender->mainScreen, SDL_SRCALPHA, 0xFF);

		pCenter.x = centerX;
		pCenter.y = centerY;
		//primMaker.drawCircle(pCenter, 16);

		rk4_th->join();	


		//parse objects	
		parseObjectsToSurface();	
		
		//parse pixels
		parseSelectPixToSurface();

		//parse players
		parsePlayersToSurface();

                //text_driver->font.back().character_set['h']->getSprite() );

		//test for boundry conditions of screen centering	
		if(centerX < screen_centX)
			xOrigin = 0;
		else if(centerX > (game_width-screen_centX))
			xOrigin = game_width - WIDTH;
		else
			xOrigin = centerX - screen_centX;

		if(centerY < screen_centY)
			yOrigin = 0;
		else if(centerY > (game_height-screen_centY))
			yOrigin = game_height - HEIGHT;
		else
			yOrigin = centerY - screen_centY;
		//std::thread rk4(&RuPPAT::RK4,this,t,dt);
		rk4_th = new std::thread(&RuPPAT::RK4,this,t,dt);

		//blit top layer
		mainRender->applySurface(xOrigin,yOrigin,backgroundLayers[2]);

        mainRender->putSprite(300, 300,
                        test_text.surface);
		


		//mainRender->putSprite(xOrigin + (WIDTH/2),yOrigin + (HEIGHT/2),primitives);
		mainRender->OnRender(xOrigin,yOrigin);

	if(nextTick > SDL_GetTicks())SDL_Delay(nextTick - SDL_GetTicks());
		nextTick = SDL_GetTicks() + interval;

	  }
	cout<<"Render thread ending!"<<endl;
//}}}
}

//------------parseSelectPixToSurface--TS
//takes everything in the pixel list and
//places it to the surface
void RuPPAT :: parseSelectPixToSurface()
{
//{{{
   pthread_rwlock_rdlock(&pix_rw_lock);

	int x, y, i, size = pixelList_m.size(), screenID=0;
	Uint32 color;	

 //#pragma omp parallel for private(x,y,color, i, j)
	 for( i=0 ; i< size ; i++)
		{
			x =	pixelList_m[i].x;// - centerX;
			y =	pixelList_m[i].y;// - centerY;
			color =	pixelList_m[i].color;
	
			mainRender->putPixel(x,y,color,screenID);
		}

   pthread_rwlock_unlock(&pix_rw_lock);
//}}}
}

//------------parsePlayersTo Surface--TS
//tkaes all the player object in the player vector
//and places them to the screen (at appropriate coords)
void RuPPAT :: parsePlayersToSurface()
{
//{{{
   pthread_rwlock_wrlock(&object_rw_lock);

	int x, y, i, color, size=players.size(), screenID=0;
	int x_aux, y_aux;
	CoOrd tempCoOrd;
	Renderables_Cont tempRenderables;
	tempCoOrd.x=0;
	tempCoOrd.y=0;
	SDL_Surface *refSurf;

 //#pragma omp parallel for private(x,y,color, i, j)
	for( i=0 ; i< size ; i++)
    {
		//Get renderable set from player (ex: missiles)
		while((players[i]->GetNextAuxDrawInfo(tempRenderables)) )
        {
				mainRender->putSprite(tempRenderables.sprites);
				mainRender->putPixel(tempRenderables.pixels);
        }
		
        //get x and y of player and center screen on player
		centerX = x = players[i]->getX();
		centerY = y = players[i]->getY();
	
		//put a selection primitive around sprite if selected
		if(players[i]->updateSprite())
		{
            //place selection primitive
			mainRender->putSprite(x,
								  y,
								  primitive_maker->Get_Cached(i));

            //place player sprite
			mainRender->putSprite(x,
								  y,
								  players[i]->getSprite());
		
		}
		else
        {
            //place player sprite
			mainRender->putSprite(x,
                                  y,
								  players[i]->getSprite());
        }
    }

   pthread_rwlock_unlock(&object_rw_lock);
//}}}
}

void RuPPAT :: parseObjectsToSurface()
{
//{{{
	int x, y, i, size;

    pthread_rwlock_rdlock(&object_rw_lock);
        size = objectList.size();

        for(i=0; i<size;i++)
            {
                x = objectList[i]->getX();
                y = objectList[i]->getY();	

                objectList[i]->sprite.updateSprite();
                mainRender->putSprite(x,y,objectList[i]->getSprite());
            }
   pthread_rwlock_unlock(&object_rw_lock);
//}}}
}




//----these below should be replaced by a better system
//for handling player control
void RuPPAT :: accelPlayer(const int p_ID, const bool isForward)
{
//{{{
	//declare the base pixel used for exhaust
	Pixel_desc t_pix;

	//setAccelVectors needs write lock, get exhaust as well
	pthread_rwlock_wrlock(&object_rw_lock);
		players[p_ID]->setAccelVectors(isForward);
		players[p_ID]->getExhaustVectors(t_pix.xVel, t_pix.yVel);
	pthread_rwlock_unlock(&object_rw_lock);
		
		//exhaust color starts red
		t_pix.color=0xff0023ff;
		t_pix.xAcc = 0;
		t_pix.yAcc = 0;
		t_pix.accelLength = 2;
		t_pix.dimFactor = 6;
		t_pix.dimTimer = 0;
		t_pix.deleteMe = false;
		t_pix.updated = 0;

	
	float origXvel = t_pix.xVel, origYvel = t_pix.yVel;
	//	cout<<origXvel<<"  "<<origYvel<<endl;	
	
	//create 10 pixels with slight random variation
	for(int i =0; i< 10; i++)
	{	
		t_pix.color += 0x000300;
		t_pix.xVel =(origXvel*60)+rand()%20 -rand()%20;
		t_pix.yVel =(origYvel*60)+rand()%20 - rand()%20;
		t_pix.x=players[p_ID]->getX()+rand()%2 - rand()%2;
		t_pix.y=players[p_ID]->getY()+rand()%2 - rand()%2;
		t_pix.x +=origXvel*10;
		t_pix.y +=origYvel*10;

        createPixElement(&t_pix);
	}
//}}}
}


void RuPPAT :: turnPlayer(const int p_ID, 
                          const bool isLeft,
                          int numTurns)
{
//{{{
	while(numTurns-- > 0)
	{
		if(isLeft)
		{
			pthread_rwlock_wrlock(&object_rw_lock);
				players[p_ID]->incrementRotationRate();
			pthread_rwlock_unlock(&object_rw_lock);
		}
		else
		{
			pthread_rwlock_wrlock(&object_rw_lock);
				players[p_ID]->decrementRotationRate();
			pthread_rwlock_unlock(&object_rw_lock);
		}
	}
//}}}
}


//This function is intended to work like a PID loop (proportional, integral, derivative)
// 	->the current implementation is a basterdized, semi-working version
// The goal is to continually adjust the turn amount until the ship is pointing towards
// the desired coordinates. Thus the manipulated variable is the rotation rate, and the
// set point is a heading or vector alignment
void RuPPAT :: turnPlayerToCoord(const int p_ID,
                                 int x,
                                 int y,
                                 const int rate)
{
//{{{
	x += xOrigin;
	y += yOrigin;

	float playerDegree = 0.0,deg_playerToPoint = 0.0, 
		tempErrAccum = 0.0, tempErrDiff = 0.0, angleDifScale=.01;
	int x_diff = 0,  y_diff=0, rateLim = 3;

		//get some values out of the player object	
		pthread_rwlock_wrlock(&object_rw_lock);
	
			playerDegree = players[p_ID]->getAngle();
	
			tempErrAccum = players[p_ID]->rotationalErrorAccum;
			tempErrDiff = players[p_ID]->lastErr;

			x_diff = x - players[p_ID]->getX();
			y_diff = y - players[p_ID]->getY();
	
		pthread_rwlock_unlock(&object_rw_lock);

	//y is upside down compared to normal coordinate plane
	y_diff *= -1;


	//1st coordinate
	if( (x_diff > 0) && (y_diff > 0))
	{
	    deg_playerToPoint = (180.0/3.141)*atan((float)y_diff/(float)x_diff);
	}
	else if( (x_diff < 0) && (y_diff > 0))
	{
	    deg_playerToPoint = 180.0 + ((180.0/3.141)*atan((float)y_diff/(float)x_diff));
	}
	else if( (x_diff < 0) && (y_diff < 0))
	{
	    deg_playerToPoint = 180.0 + ((180.0/3.141)*atan((float)y_diff/(float)x_diff));
	}
	else if( (x_diff > 0) && (y_diff<0))
	{
	    deg_playerToPoint = 360.0 + ((180.0/3.141)*atan((float)y_diff/(float)x_diff));
	}


    float angle_diff = deg_playerToPoint - playerDegree;
    printf("DIFFERENCE: %f\n", angle_diff);

    //tempErrAccum += angle_diff*.01;
    float dif_rate = 0.0;
    float pos_curr_rate = 0.0;

	pthread_rwlock_wrlock(&object_rw_lock);
		float curr_rate = players[p_ID]->getRotationRate(); 
	pthread_rwlock_unlock(&object_rw_lock);

    tempErrDiff =  angle_diff*.1 - tempErrDiff; //angle_diff - tempErrDiff;

	if(curr_rate<0)
		 pos_curr_rate = curr_rate * -1;
	else
		 pos_curr_rate = curr_rate;

	dif_rate = ((float)rate - pos_curr_rate)*.13;
	if(angle_diff<0)
	{
		dif_rate -= tempErrDiff*.4;
		dif_rate += tempErrAccum*.1/dif_rate;
		dif_rate -= (angle_diff*.003);
	}
	else
	{
		dif_rate += tempErrDiff*.4;
		dif_rate += tempErrAccum*.1/dif_rate;
		dif_rate -= (angle_diff*.003);
	}
	if(dif_rate<0)dif_rate = dif_rate* -1.0;
	printf(" Acumulated Err: %f \t Error Diff: %f \t DIFF RATE: %f\n",
		tempErrAccum, tempErrDiff, dif_rate);

	if(angle_diff > 4 )
	{
		//point is to left of player	
		if( angle_diff< 180 && curr_rate<rateLim)
		{
				tempErrAccum -= angle_diff*angleDifScale;
			printf("TURNING LEFT\n");
				turnPlayer(p_ID, true, dif_rate);	
		}
		//closest to right
		else if( (angle_diff > 180) && curr_rate<rateLim)
		{

				tempErrAccum -= (360-angle_diff)*angleDifScale;
			printf("TURNING RIGHT!\n");
				turnPlayer(p_ID, false, dif_rate); 
		}
	}
	else if(angle_diff < -4 )
	{
		if ((-1.0*angle_diff) > 180 && curr_rate<rateLim)
		{
			tempErrAccum -= (360-angle_diff)*angleDifScale;
			printf("TURNING LEFT\n");
			turnPlayer(p_ID, true, dif_rate);
		}
		else if ((-1.0*angle_diff) <180 && curr_rate<rateLim)
		{
			tempErrAccum -= angle_diff*angleDifScale;
			printf("TURNING RIGHT\n");
			turnPlayer(p_ID, false, dif_rate);
		}
	}
	else
	{
	//tempErrAccum = tempErrAccum*.1;
		pthread_rwlock_wrlock(&object_rw_lock);

		players[p_ID]->setRotationRate(0);
	
		pthread_rwlock_unlock(&object_rw_lock);

	}
		pthread_rwlock_wrlock(&object_rw_lock);
	
//			playerDegree = players[p_ID]->getAngle();
	
			players[p_ID]->rotationalErrorAccum = tempErrAccum;
			players[p_ID]->lastErr = tempErrDiff;

	//		x_diff = x - players[p_ID]->getX();
	//		y_diff = y - players[p_ID]->getY();
	
		pthread_rwlock_unlock(&object_rw_lock);
//}}}
}


void RuPPAT :: firePlayersWeapon(const int p_ID)
{
	players[p_ID]->fireSelectedMissile();
}



//------------createPixElement-----TS
//pushes the pixel descriptor to the pixel
//list
void RuPPAT :: createPixElement(Pixel_desc *pixel )
{	
//{{{
    pthread_rwlock_wrlock(&pix_rw_lock);

        pixelList_m.push_back(*pixel);

    pthread_rwlock_unlock(&pix_rw_lock);
//}}}
}

//-----------------createPixElemenent-------
//create a pix descriptor from arguments and
//then push it to the pixel list vector
Pixel_desc tmpPix;	
void RuPPAT :: createPixElement(const int x, const int y,
                                const int color,
                                const int xAccel,
                                const int yAccel,
                                const int accLength,
                                const int dimFactor,
                                const int mass )
{	
//{{{
		//assign location
		tmpPix.x=x;
		tmpPix.y=y;
		
		//assign color	
		tmpPix.color=color; 
	
		//if it has mass
		if(mass!=0)
		{//assign amss and add to massList
		tmpPix.mass = mass;
	  	//massList.push_back(topMap[x][y]);
	  	}

		//intit velocity to zero, set acce
		tmpPix.xVel = 0;
		tmpPix.yVel = 0;
		tmpPix.xAcc = xAccel;
		tmpPix.yAcc = yAccel;

		tmpPix.deleteMe = false;
		tmpPix.ID=baseID++;

		//set accellength:number of times to apply this accel to vel
		tmpPix.accelLength = accLength;
		tmpPix.updated = 0;

		//set dimfactor:higher=pixel fades away quicker
		tmpPix.dimFactor = dimFactor;

		 //finally, push tmp to vector
		 pthread_rwlock_wrlock(&pix_rw_lock);
		    pixelList_m.push_back(tmpPix);
		 pthread_rwlock_unlock(&pix_rw_lock);
//}}}
}

//------------setUpdateOnSelectPix-----
//assigns "set" argument to all elements
//in pixel lists updated memeber
void RuPPAT :: setUpdateOnSelectPix(const int set)
{
//{{{
//#pragma omp parallel for
  for(int i=0 ; i<pixelList_m.size() ; i++)
	{
		pixelList_m[i].updated = set;
	}
//}}}
}

//------------handleAllDeleteME--------->NEED WORK
//Pixels should add themselves to list for deletion
//so the program doesnt have to search for them
//
//go through pix list and delete everything 
//with deleteMe set high
void RuPPAT :: handleAllDeleteMe()
{
//{{{
  //remove pixelList elements that have deleteMe set
  pthread_rwlock_wrlock(&pix_rw_lock);
  for(int k=0;k<pixelList_m.size();k++)
	{
	if(pixelList_m[k].deleteMe)
	  {
		pixelList_m.erase(pixelList_m.begin()+k);
		k--;//compensate for the resize
	  }
	}
  pthread_rwlock_unlock(&pix_rw_lock);
//}}}
}

//------------handleDelete-------------
//remove pixelList element that has deleteMe set
//can do other things for delete later
void RuPPAT :: handleDelete(const int k)
{
//{{{
	//CURRENTLY LOCKS OUTSIDE OF FUNCTION
	//pthread_rwlock_wrlock(&pix_rw_lock);
		pixelList_m.erase(pixelList_m.begin()+k);
	//pthread_rwlock_unlock(&pix_rw_lock);
//}}}
}

