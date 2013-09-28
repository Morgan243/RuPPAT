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
//#include <thread>
//#include <mutex>
#include <omp.h>
#include <unistd.h>
#include "PhysFuncs.h"


using namespace std;


//------------CONSTRUCTOR--------
RuPPAT :: RuPPAT(struct RuPPAT_Options options)
{
//{{{
    
    initLocks();

	//start up the renderer
	mainRender = new Render(options.width,
                             options.height, 
                             options.bpp,
                             options.flags);

    Sprite::render_context = mainRender->renderer;

    SDL_Surface *tempBkg, * tempBkgOpt;

    //load the background sprites
	for( int i = 0; i< options.background_paths.size(); i++)
	{
        SDL_Texture *tempBkg_text, *tempBkgOpt_text;
		
		cout<<"Loading..."<<options.background_paths[i]<<endl;

		tempBkg = IMG_Load((char *)options.background_paths[i].c_str());
		tempBkg_text = SDL_CreateTextureFromSurface(mainRender->renderer, tempBkg);
	
		cout<<"\tReassigning..."<<options.background_paths[i]<<endl;

		cout<<"\tPushing back..."<<options.background_paths[i]<<endl;
		backgroundLayers.push_back(tempBkg_text);	
	}

    mainRender->low_composite.push_back(backgroundLayers[0]);
    mainRender->low_composite.push_back(backgroundLayers[1]);
    mainRender->high_composite.push_back(backgroundLayers[2]);    

    game_height = tempBkg->h;
    game_width = tempBkg->w;

    //make sure common has the width and height values
	Common::SetDimensions(game_width, game_height);

    //let render know the boundries
	mainRender->setGameArea(game_width, game_height);

	printf("game width = %d, game height = %d\n", game_width, game_height);

	WIDTH = options.width;
	HEIGHT = options.height;

    screen_centX=WIDTH/2;
    screen_centY=HEIGHT/2;
    //primitive factory
	primitive_maker = new Primitives(1,
		   							0x55efe000,
								   	mainRender->pre_surface);

    //hard coded font file, need to get this from the config
    //however, not bothering to due to a planned structure overhaul
//    this->text_driver = new TextDriver("main", "FontSheet.png",
//                                        25, 25, 12, 12); //width, height, rows, columns

    if(options.style == LEVEL_STYLE::WRAP)
    {

    }
    else 
    {
        this->gfxPlacer = &RuPPAT::bounce_gfxPlacer;
    }
	gravitationalConstant = 22;
//}}}
}


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

    Sprite::render_context = mainRender->renderer;

    SDL_Surface *tempBkg, * tempBkgOpt;

    //load the background sprites
	for( int i = 0; i< bkg_paths.size(); i++)
	{
        SDL_Texture *tempBkg_text, *tempBkgOpt_text;
		
		cout<<"Loading..."<<bkg_paths[i]<<endl;

		tempBkg = IMG_Load((char *)bkg_paths[i].c_str());
		tempBkg_text = SDL_CreateTextureFromSurface(mainRender->renderer, tempBkg);
	
		cout<<"\tReassigning..."<<bkg_paths[i]<<endl;
		//tempBkgOpt = SDL_DisplayFormatAlpha(tempBkg);
		
		//SDL_SetAlpha(tempBkgOpt, SDL_SRCALPHA, 0xFF);

		//SDL_FreeSurface(tempBkg);

		cout<<"\tPushing back..."<<bkg_paths[i]<<endl;
		backgroundLayers.push_back(tempBkg_text);	
	}

    mainRender->low_composite.push_back(backgroundLayers[0]);
    mainRender->low_composite.push_back(backgroundLayers[1]);
    mainRender->high_composite.push_back(backgroundLayers[2]);    

    //hacky, but a way to create a proper pre-surface...Copy it!
//    mainRender->pre_surface = SDL_ConvertSurface(tempBkg, tempBkg->format, tempBkg->flags);
//    SDL_SetSurfaceBlendMode(mainRender->pre_surface, SDL_BLENDMODE_BLEND);

    //base the game boundries based on the height and width
//	game_width = backgroundLayers[0]->width;
//	game_height = backgroundLayers[0]->height;
    game_height = tempBkg->h;
    game_width = tempBkg->w;

    //make sure common has the width and height values
	Common::SetDimensions(game_width, game_height);

    //let render know the boundries
	mainRender->setGameArea(game_width, game_height);

	printf("game width = %d, game height = %d\n", game_width, game_height);

	WIDTH = width;
	HEIGHT = height;
    screen_centX=WIDTH/2;
    screen_centY=HEIGHT/2;

    //primitive factory
	primitive_maker = new Primitives(1,
		   							0x55efe000,
								   	mainRender->pre_surface);

    //hard coded font file, need to get this from the config
    //however, not bothering to due to a planned structure overhaul
//    this->text_driver = new TextDriver("main", "FontSheet.png",
//                                        25, 25, 12, 12); //width, height, rows, columns

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
		objectList.back()->sprite->setRotationRate(rotationRate);

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

        new_object->sprite->getDimensions(width, height);

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

//-------------RK4_force--------------
// 	A new new implementation of the
// RK4_all functionality. Using force calculations
// instead of acceleration based calculations will 
// lead to a more flexible and efficient phys engine
void RuPPAT::RK4_force(const float t, const float dt)
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

    if(num_objects == 1)
    {
		//Runge Kutta integrator
		pthread_rwlock_wrlock(&object_rw_lock);	

			//Get the outer loop objects descriptor
			obj_desc_tri.mass = 0;
            obj_desc_tri.x = 0;
            obj_desc_tri.y = 0;
		
			//Apply outer loop object [i] to the current object
			obj_desc_prim = objectList[i]->PhysicsHandler_force(t, dt, obj_desc_tri);

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
    else 
    {
        //go through the object list to apply grav to other objects
        for(int j = i+1; j<num_objects; j++)
        {
            //Runge Kutta integrator
            pthread_rwlock_wrlock(&object_rw_lock);	

                //Get the outer loop objects descriptor
                obj_desc_tri = objectList[j]->getDescriptor();
            
                //Apply outer loop object [i] to the current object
                obj_desc_prim = objectList[i]->PhysicsHandler_force(t, dt, obj_desc_tri);

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

    objectList[i]->updatePositional(t, dt);

	//next, go through the pixel structs 
	for(int j = 0; j<num_pixels; j++)
	{

		pthread_rwlock_rdlock(&object_rw_lock);
		 pthread_rwlock_wrlock(&pix_rw_lock);

			obj_desc_sec = &pixelList_m[j];
			obj_desc_sec = objectList[i]->PhysicsHandler_force(*obj_desc_sec, t, dt);

         pthread_rwlock_unlock(&pix_rw_lock);	
        pthread_rwlock_unlock(&object_rw_lock);

            //do this once and last!
            if(i == num_objects - 1)
            {
            pthread_rwlock_rdlock(&object_rw_lock);
             pthread_rwlock_wrlock(&pix_rw_lock);
                PhysFunc::integrate_force(*obj_desc_sec, t, dt);

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
 }
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
void RuPPAT :: RunRuPPAT()
{
//{{{
    Uint32 T1 = SDL_GetTicks(),
           T2 = 1000;

    engine_rate = 160;
     //interval = 1000/engine_rate,
     interval = calcTickInterval(engine_rate);
     nextTick = SDL_GetTicks() + interval;

     int num_jobs = 0;

    RenderJob_Container *current_job;

    //physics thread
    rk4_th = new std::thread(&RuPPAT::RK4_force,this,t,dt);

	//keep looping until program end
	while(!done)
	{
		//increment time
		t += dt;

        //only the main thread can use the renderer, so we have render jobs queue
        //this implementation will only do one job per cylce
        if(num_jobs = this->render_jobs.size())
        {
            //get the front (top) job
            current_job = render_jobs.front();
            render_jobs.pop();

            //what is the job
            if(current_job->id == JOB_ID::COPY_SPRITE)
            {
                //get job container's sprite to be copied and construct a new sprite
                Sprite *tmpSprite = new Sprite(((Sprite*)current_job->opt_container));
                //return the copied sprite to the job
                current_job->return_obj = (void*)tmpSprite;
            }
        }
	    
        //points to a function that will determine where to place things
        //this function must also handle the joining and re-launching rk4 thread
        (*this.*gfxPlacer)();


        //sleep out until nex tick
        if(nextTick > SDL_GetTicks())
            SDL_Delay(nextTick - SDL_GetTicks());

        nextTick = SDL_GetTicks() + interval;
	}

	cout<<"Main thread terminating....!"<<endl;
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
				mainRender->putSprite(tempRenderables.sprites_text);
				mainRender->putPixel(tempRenderables.pixels);
        }
		
        //get x and y of player and center screen on player
		centerX = x = players[i]->getX();
		centerY = y = players[i]->getY();
	
        //cout<<"X,Y ----> ("<<x<<","<<y<<")\n";

		//put a selection primitive around sprite if selected
		if(players[i]->updateSprite())
		{
            //place selection primitive
//			mainRender->putSprite(x,
//								  y,
//								  primitive_maker->Get_Cached(i));

            //place player sprite
			mainRender->putSprite(x,
								  y,
								  players[i]->getSprite_text());
		
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

                objectList[i]->sprite->updateSprite();
                mainRender->putSprite(x,y,objectList[i]->getSprite_text());
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
		players[p_ID]->setForceVectors(isForward);
		players[p_ID]->getExhaustVectors(t_pix.xForce, t_pix.yForce);
	pthread_rwlock_unlock(&object_rw_lock);

        t_pix.xForce *= .1;
        t_pix.yForce *= .1;

        cout<<"Pixel X Force = " << t_pix.xForce <<endl;
        cout<<"Pixel Y Force = " << t_pix.yForce <<endl;
		
		//exhaust color starts red
		t_pix.color=0xff0023ff;
		t_pix.xAcc = 0;
		t_pix.yAcc = 0;
		t_pix.accelLength = 1;
		t_pix.dimFactor = 5;
		t_pix.dimTimer = 0;
		t_pix.deleteMe = false;
		t_pix.updated = 0;
        t_pix.mass = 1;

	
	float origXvel = t_pix.xVel, origYvel = t_pix.yVel;
	
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
//{{{
    RenderJob_Container *fire_missile_job;
    fire_missile_job = new RenderJob_Container;
    fire_missile_job->id = JOB_ID::COPY_SPRITE;

    //get the missile sprite we need to copy, cast to void pointer
    fire_missile_job->opt_container = (void*)players[p_ID]->getSelectedMissile()->sprite;

    //add to the render thread's job queue
    this->render_job_mutex.lock();
        this->render_jobs.push(fire_missile_job);
    this->render_job_mutex.unlock();

    //wait for render to finish the job!
    while(fire_missile_job->return_obj == NULL)
        usleep(50);

    players[p_ID]->fireSelectedMissile((Sprite*)fire_missile_job->return_obj);
//}}}
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

void RuPPAT :: bounce_gfxPlacer(void)
{
//{{{
    //blit bottom layer	
    //mainRender->applySurface(xOrigin/1.2,yOrigin/1.2,backgroundLayers[0]);
    mainRender->applyTextureToMain(xOrigin/1.2, yOrigin/1.2, backgroundLayers[0]);
    
    //blit middle layer
    //mainRender->applySurface(xOrigin,yOrigin,backgroundLayers[1]);
    mainRender->applyTextureToMain(xOrigin, yOrigin, backgroundLayers[1]);


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
    
    rk4_th = new std::thread(&RuPPAT::RK4_force,this,t,dt);

    //blit top layer
    //mainRender->applySurface(xOrigin,yOrigin,backgroundLayers[2]);

    //mainRender->putSprite(300, 300, test_text.surface);
    mainRender->applyTextureToMain(xOrigin, yOrigin, backgroundLayers[2]);

    //mainRender->putSprite(xOrigin + (WIDTH/2),yOrigin + (HEIGHT/2),primitives);
    mainRender->OnRender(xOrigin,yOrigin);
//}}}
}

void RuPPAT :: wrap_gfxPlacer(void)
{
//{{{
    //blit bottom layer	
    //mainRender->applySurface(xOrigin/1.2,yOrigin/1.2,backgroundLayers[0]);
    mainRender->applyTextureToMain(xOrigin/1.2, yOrigin/1.2, backgroundLayers[0]);
    
    //blit middle layer
    //mainRender->applySurface(xOrigin,yOrigin,backgroundLayers[1]);
    mainRender->applyTextureToMain(xOrigin, yOrigin, backgroundLayers[1]);

    rk4_th->join();	

    //parse objects	
    parseObjectsToSurface();	
    
    //parse pixels
    parseSelectPixToSurface();

    //parse players
    parsePlayersToSurface();

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

    rk4_th = new std::thread(&RuPPAT::RK4,this,t,dt);

    //mainRender->putSprite(300, 300, test_text.surface);
    mainRender->applyTextureToMain(xOrigin, yOrigin, backgroundLayers[2]);

    //mainRender->putSprite(xOrigin + (WIDTH/2),yOrigin + (HEIGHT/2),primitives);
    mainRender->OnRender(xOrigin,yOrigin);
    //mainRender->OnRender();
//}}}
}

int RuPPAT::calcTickInterval(int engineRate)
{
//{{{
    this->engine_rate = engineRate;
    this->interval = 1000/engineRate;
    return this->interval;
//}}}
}

int RuPPAT::adjustRate(int delta)
{
//{{{
    this->engine_rate += delta;
    this->interval = 1000/this->engine_rate;
    return this->interval;
//}}}
}

int RuPPAT::adjust_dt(float delta)
{
    this->dt += delta;
}
