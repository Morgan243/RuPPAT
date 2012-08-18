/*	Rudimentary Particle, Physics, and Time Engine
 *			>>>RuPPAT<<<
 *---As it implies, this object contains functions to not only calculate
 * the physics of sprites and pixels in a given time frame, but also render
 * them properly 
*/

#include "RuPPAT.h"
#include "PhysFuncs.h"
#include "Player.h"
#include "Object.h"
#include <pthread.h>
#include <omp.h>
#include <unistd.h>

//mutexes/lock used throughout
pthread_mutex_t  pix_list_lock_2;
pthread_rwlock_t  pix_rw_lock, 
		  mass_rw_lock,
		  ent_rw_lock,
		  player_rw_lock,
		  object_rw_lock;

int WIDTH, HEIGHT;

using namespace std;
	bool done;//if this goes true, all threads stop, engine stops

	//main window
	Render* mainRender;

	
	vector<Pixel_desc> pixelList;//independent pixels on screen
	vector<Pixel_desc> pixelList_m;//points of mass on screen

	
	vector<Entity_desc> entList;//indpenedt sprites on scree
	vector<Entity_desc> entList_m;//sprites that attract others 

	vector<SDL_Surface *>backgroundLayers;//layers of the background

	vector<Player *> players;//independent sprite that controlled by AI or human

	vector<Object *> objectList;

	//unused as of now
//	vector<Mass_desc> pMassList;



	queue<Pixel_desc> toRender;
	


//------------CONSTRUCTOR--------
//initialize some locks, start render (window)
//and set some variables
RuPPAT :: RuPPAT(int width,int height,int bpp, unsigned int flags,
		string background_img_paths[], int num_bks)
{
	
	//init all the locks--ERROR CHECK too 
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



	//start the render; opens up game window
	mainRender = new Render(width, height, BPP, flags );

	//load all the layers for the background
	for(int i = 0; i<num_bks;i++)
	{
	SDL_Surface *tempBkg, * tempBkgOpt;

		tempBkg = IMG_Load((char *)background_img_paths[i].c_str());

		tempBkgOpt = SDL_DisplayFormatAlpha(tempBkg);

		backgroundLayers.push_back(tempBkgOpt);	

	}

	//engine needs to know how much real estate it has
	WIDTH = width;
	HEIGHT = height;


	//set grav constant
	gravitationalConstant=22;

}


//------------DECONSTRUCTOR-------
//destroy the mutexes
RuPPAT :: ~RuPPAT()
{
	pthread_mutex_destroy(&pix_list_lock_2);
	pthread_rwlock_destroy(&pix_rw_lock);
	pthread_rwlock_destroy(&mass_rw_lock);
	pthread_rwlock_destroy(&ent_rw_lock);
	pthread_rwlock_destroy(&player_rw_lock);
}


//------------createPixElement-----TS
//pushes the pixel descriptor to the pixel
//list
void RuPPAT :: createPixElement(Pixel_desc *pixel )
{	
  Pixel_desc temp_pix = *pixel;

   pthread_rwlock_wrlock(&pix_rw_lock);

  	pixelList_m.push_back(temp_pix);

   pthread_rwlock_unlock(&pix_rw_lock);
}


//------------parseSelectPixToSurface--TS
//takes everything in the pixel list and
//places it to the surface
void RuPPAT :: parseSelectPixToSurface()
{
   pthread_rwlock_rdlock(&pix_rw_lock);

	int x, y, i, j, color, size=pixelList_m.size(), screenID=0;
		
 //#pragma omp parallel for private(x,y,color, i, j)
	 for( i=0 ; i< size ; i++)
		{
		x =	pixelList_m[i].x;
		y =	pixelList_m[i].y;
		color =	pixelList_m[i].color;
	
		mainRender->putPixel(x,y,color,screenID);
		}

   pthread_rwlock_unlock(&pix_rw_lock);
}

//------------parsePlayersTo Surface--TS
//tkaes all the player object in the player vector
//and places them to the screen (at appropriate coords)
void RuPPAT :: parsePlayersToSurface()
{
   pthread_rwlock_rdlock(&player_rw_lock);

	int x, y, i, j, color, size=players.size(), screenID=0;

//use line below to use OpenMP to launch a team of threads on the vector
 //#pragma omp parallel for private(x,y,color, i, j)
	 for( i=0 ; i< size ; i++)
		{
		x =	players[i]->getX();
		y =	players[i]->getY();
		
		players[i]->updateSprite();
	
		mainRender->putSprite(x,y, players[i]->getSprite());
		}

   pthread_rwlock_unlock(&player_rw_lock);
}

void RuPPAT :: parseObjectsToSurface()
{
	int x, y, i;
   pthread_rwlock_rdlock(&object_rw_lock);
	int size=objectList.size();

	for(i=0; i<size;i++)
		{
			x = objectList[i]->getX();
			y = objectList[i]->getY();	

			mainRender->putSprite(x,y,objectList[i]->getSprite());

		}

   pthread_rwlock_unlock(&object_rw_lock);


}



//------------setUpdateOnSelectPix-----
//assigns "set" argument to all elements
//in pixel lists updated memeber
void RuPPAT :: setUpdateOnSelectPix(int set)
{
//#pragma omp parallel for
  for(int i=0 ; i<pixelList_m.size() ; i++)
	{
		pixelList_m[i].updated = set;
	}

}


//------------handleAllDeleteME--------->NEED WORK
//Pixels should add themselves to list for deletion
//so the program doesnt have to search for them
//
//go through pix list and delete everything 
//with deleteMe set high
void RuPPAT :: handleAllDeleteMe()
{
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
}


//------------handleDelete-------------
//remove pixelList element that has deleteMe set
//can do other things for delete later
void RuPPAT :: handleDelete(int k)
{
	//CURRENTLY LOCKS OUTSIDE OF FUNCTION
	//pthread_rwlock_wrlock(&pix_rw_lock);
		pixelList_m.erase(pixelList_m.begin()+k);
	//pthread_rwlock_unlock(&pix_rw_lock);
}


//------------applyDimming--------------NEED WORK
//Pixels fade to black when they should be using
//some kind of alpha blending or blending with
//the backgroun
//
//dims a pixel struct, returns 1 if it is deleted do to dimming out
int RuPPAT :: applyDimming(Pixel_desc &pix_t)
{
 const int dim_scale = 1; //bring this up to increase max dim length
 int dimFactor = pix_t.dimFactor;
 int current_color = pix_t.color;
 unsigned int dimTimer= pix_t.dimTimer;
	
	//if pixel is to be dimmed and dimTimer is over factor
	if(dimFactor &&  ++dimTimer>dimFactor)
	{
		//reset the dimtimer
		dimTimer = 0;

		//separate out the colors for manipulation
		unsigned int tmpR = (current_color & 0xFF0000)>>16;
		unsigned int tmpG = (current_color & 0x00FF00)>>8;
		unsigned int tmpB = (current_color & 0x0000FF);
	
		//dim RED
		if(tmpR <10)
			{tmpR=0;}
			else
			{tmpR/=1.2;}

		//dim GREEN
		if(tmpG <10)
			{tmpG=0;}
			else
			{tmpG/=1.2;}

		//dum BLUE
		if(tmpB <10)
			{tmpB=0;}
			else
			{tmpB/=1.2;}

		//recombine the colors
		pix_t.color = ( (tmpR<<16) | (tmpG<<8) | tmpB);

	}
	//reassign timer
	pix_t.dimTimer = dimTimer;

	//if the color is really dark, just remove the pixel
	if(pix_t.color < 10)
		{pix_t.deleteMe = true;}
}//END <applyDimming>


int RuPPAT :: addPlayer(string spritePath, int numRotations, int startingAngle,
			float maxAccel, int x, int y)
{
	int size;

		pthread_rwlock_wrlock(&player_rw_lock);	
			players.push_back(new Player(spritePath, numRotations, startingAngle,
						maxAccel, x, y));
			size = players.size()-1;

		pthread_rwlock_unlock(&player_rw_lock);

	return size;
}

//returns entity ID
int RuPPAT :: addEntity(Entity_desc new_ent)
{
	int size;

		pthread_rwlock_wrlock(&ent_rw_lock);
			entList.push_back(new_ent);	
			size = entList.size()-1;
		pthread_rwlock_unlock(&ent_rw_lock);

	return size;
}


int RuPPAT :: addObject(string spritePath, int x, int y, int mass)
{
	int size;

		pthread_rwlock_wrlock(&object_rw_lock);
			objectList.push_back(new Object(spritePath, x, y, mass));
		pthread_rwlock_unlock(&object_rw_lock);

	return size;
}

Entity_desc RuPPAT :: RK4_entity(Entity_desc ent)
{

	const Uint32 NOW = SDL_GetTicks(); 
	float newX, newY, tmp_xVel, tmp_yVel;

	Mass_desc temp_object;	

	//get size of mass
//	pthread_rwlock_rdlock(&mass_rw_lock);
//	 int mass_size = pMassList.size();	
//	pthread_rwlock_unlock(&mass_rw_lock);

	pthread_rwlock_rdlock(&object_rw_lock);
	 int object_size = objectList.size();	
	pthread_rwlock_unlock(&object_rw_lock);





	const int timerConst = 1;
	Entity_desc temp_e = ent;
	Pixel_desc temp_p;
   for(int j = 0; j<object_size;j++)
	{

	//get mass
	pthread_rwlock_rdlock(&object_rw_lock);
		temp_object = objectList[j]->getDesc();	
	pthread_rwlock_unlock(&object_rw_lock);




		integrate_ent(temp_e, 0.0, 0.001,
			temp_object.mass, temp_object.xLoc, temp_object.yLoc);

		newX = temp_e.x;
		newY = temp_e.y;

		tmp_xVel = temp_e.xVel;
		tmp_yVel = temp_e.yVel;


			if(newX<=0)
				{newX=1;tmp_xVel=tmp_xVel*(-1);}

			if(newX>=WIDTH)
				{newX=WIDTH-1;tmp_xVel=tmp_xVel*(-1);}

			if(newY<=0)
				{newY=1;tmp_yVel=tmp_yVel*(-1);}

			if(newY>=HEIGHT)
				{newY=HEIGHT-1;tmp_yVel=tmp_yVel*(-1);}	



			temp_e.xVel = tmp_xVel;
			temp_e.yVel = tmp_yVel;
			temp_e.x = newX;
			temp_e.y = newY;

	



	    
	}//end massLIst for[j]
return temp_e;
}


void RuPPAT :: RK4_all(float t, float dt)
{
	const Uint32 NOW = SDL_GetTicks(); 
	float newX, newY, tmp_xVel, tmp_yVel;

	Mass_desc temp_mass;	
	Pixel_desc temp_p;
	Entity_desc temp_ent;

	//get the size of the player list
	pthread_rwlock_rdlock(&player_rw_lock);
	 int player_size = players.size();
	pthread_rwlock_unlock(&player_rw_lock);

	//get the size of pixeList
	pthread_rwlock_rdlock(&pix_rw_lock);
	 int pixel_size = pixelList_m.size();
	pthread_rwlock_unlock(&pix_rw_lock);

	//get size of mass
	pthread_rwlock_rdlock(&object_rw_lock);
	 int mass_size = objectList.size();	
	pthread_rwlock_unlock(&object_rw_lock);



	const int timerConst = 1;
	
 if(mass_size)
  {
   for(int j = 0; j<mass_size;j++)
	{

	//get mass
	pthread_rwlock_rdlock(&object_rw_lock);
		temp_mass = objectList[j]->getDesc();	
	pthread_rwlock_unlock(&object_rw_lock);


	  for(int i=0; i<player_size ;i++)
	      {


		pthread_rwlock_rdlock(&player_rw_lock);
			temp_ent = players[i]->getDescriptor();
		pthread_rwlock_unlock(&player_rw_lock);

	integrate_ent(temp_ent,t, dt,
			temp_mass.mass, temp_mass.xLoc, temp_mass.yLoc);

		newX = temp_ent.x;
		newY = temp_ent.y;

		tmp_xVel = temp_ent.xVel;
		tmp_yVel = temp_ent.yVel;


			if(newX<=0)
				{newX=1;tmp_xVel=tmp_xVel*(-1);}

			if(newX>=WIDTH)
				{newX=WIDTH-1;tmp_xVel=tmp_xVel*(-1);}

			if(newY<=0)
				{newY=1;tmp_yVel=tmp_yVel*(-1);}

			if(newY>=HEIGHT)
				{newY=HEIGHT-1;tmp_yVel=tmp_yVel*(-1);}	



			temp_ent.xVel = tmp_xVel;
			temp_ent.yVel = tmp_yVel;
			temp_ent.x = newX;
			temp_ent.y = newY;

		pthread_rwlock_wrlock(&player_rw_lock);
			players[i]->setDescriptor(temp_ent);
		pthread_rwlock_unlock(&player_rw_lock);

	      }



	  for(int i=0; i<pixel_size ;i++)
	      {

	pthread_rwlock_rdlock(&pix_rw_lock);
		temp_p = pixelList_m[i];
	pthread_rwlock_unlock(&pix_rw_lock);	


		integrate(temp_p, t, dt,
			temp_mass.mass, temp_mass.xLoc, temp_mass.yLoc);

		newX = temp_p.x;
		newY = temp_p.y;

		tmp_xVel = temp_p.xVel;
		tmp_yVel = temp_p.yVel;


			if(newX<=0)
				{newX=1;tmp_xVel=tmp_xVel*(-1);}

			if(newX>=WIDTH)
				{newX=WIDTH-1;tmp_xVel=tmp_xVel*(-1);}

			if(newY<=0)
				{newY=1;tmp_yVel=tmp_yVel*(-1);}

			if(newY>=HEIGHT)
				{newY=HEIGHT-1;tmp_yVel=tmp_yVel*(-1);}	



			temp_p.xVel = tmp_xVel;
			temp_p.yVel = tmp_yVel;
			temp_p.x = newX;
			temp_p.y = newY;

	pthread_rwlock_wrlock(&pix_rw_lock);
		pixelList_m[i] = temp_p;
		if(pixelList_m[i].dimFactor){applyDimming(pixelList_m[i]);}
		if(pixelList_m[i].deleteMe)
			{
			handleDelete(i);
			pixel_size = pixelList_m.size();
			}
	pthread_rwlock_unlock(&pix_rw_lock);



	      }//end pixelList for[i]
	}//end massLIst for[j]
   }//end if mass_size not 0
 else//no masses!
  {
	for(int i=0; i<player_size ;i++)
	      {


		pthread_rwlock_rdlock(&player_rw_lock);
			temp_ent = players[i]->getDescriptor();
		pthread_rwlock_unlock(&player_rw_lock);

	integrate_ent(temp_ent,t, dt,
			0, 0, 0);

		newX = temp_ent.x;
		newY = temp_ent.y;

		tmp_xVel = temp_ent.xVel;
		tmp_yVel = temp_ent.yVel;


			if(newX<=0)
				{newX=1;tmp_xVel=tmp_xVel*(-1);}

			if(newX>=WIDTH)
				{newX=WIDTH-1;tmp_xVel=tmp_xVel*(-1);}

			if(newY<=0)
				{newY=1;tmp_yVel=tmp_yVel*(-1);}

			if(newY>=HEIGHT)
				{newY=HEIGHT-1;tmp_yVel=tmp_yVel*(-1);}	



			temp_ent.xVel = tmp_xVel;
			temp_ent.yVel = tmp_yVel;
			temp_ent.x = newX;
			temp_ent.y = newY;

		pthread_rwlock_wrlock(&player_rw_lock);
			players[i]->setDescriptor(temp_ent);
		pthread_rwlock_unlock(&player_rw_lock);

	      }



	  for(int i=0; i<pixel_size ;i++)
	      {

	pthread_rwlock_rdlock(&pix_rw_lock);
		temp_p = pixelList_m[i];
	pthread_rwlock_unlock(&pix_rw_lock);	


		integrate(temp_p, t, dt,
			0, 0, 0);

		newX = temp_p.x;
		newY = temp_p.y;

		tmp_xVel = temp_p.xVel;
		tmp_yVel = temp_p.yVel;


			if(newX<=0)
				{newX=1;tmp_xVel=tmp_xVel*(-1);}

			if(newX>=WIDTH)
				{newX=WIDTH-1;tmp_xVel=tmp_xVel*(-1);}

			if(newY<=0)
				{newY=1;tmp_yVel=tmp_yVel*(-1);}

			if(newY>=HEIGHT)
				{newY=HEIGHT-1;tmp_yVel=tmp_yVel*(-1);}	



			temp_p.xVel = tmp_xVel;
			temp_p.yVel = tmp_yVel;
			temp_p.x = newX;
			temp_p.y = newY;

	pthread_rwlock_wrlock(&pix_rw_lock);
		pixelList_m[i] = temp_p;
		if(pixelList_m[i].dimFactor){applyDimming(pixelList_m[i]);}
		if(pixelList_m[i].deleteMe)
			{
			handleDelete(i);
			pixel_size = pixelList_m.size();
			}
	pthread_rwlock_unlock(&pix_rw_lock);



	      }//end pixelList for[i]



  }//end else mass_size is zero
}

//-----------runDemos-------
//just puts pixels with different attributes
//depending on the selection argument
void RuPPAT :: runDemos(void *selection)
{
	float theta;
	int xPos=300, yPos=300, radius=4;	
	int xCent=400, yCent=400;
	char *select = (char*)selection;
	cout<<"Selection is: "<<*select<<endl;

//Create our temporary pixel
Pixel_desc t_pix;

	t_pix.color=0xff00ff;
	t_pix.xAcc = 0;
	t_pix.yAcc = 0;
	t_pix.accelLength = 2;
	t_pix.dimFactor = 0;

	t_pix.dimTimer = 0;
	t_pix.Xtimer =t_pix.Ytimer =0;// SDL_GetTicks();

	t_pix.deleteMe = false;

	t_pix.updated = 0;

	switch (*select)
	 {
	   case 'g':
	   {
			t_pix.color = rand()%0xffffff;
		int count = 0;
			t_pix.x = 747;
			t_pix.yVel = 78;
			t_pix.xAcc = 0;
			t_pix.dimFactor = 1000;
		while(count<175 && !done)
		{  

			t_pix.y = 584;
			t_pix.x-=1;
			createPixElement(&t_pix);
			t_pix.y = t_pix.y +rand()%26 - rand()%36;
			createPixElement(&t_pix);	
			t_pix.y = t_pix.y +rand()%19 - rand()%16;
			createPixElement(&t_pix);	
		count++;
		}
	    break;
	   }


	   case 'r':
	   {	int count=0;
		while(count<5 && !done)
		{
		  for(int i = 0; i<WIDTH ; i+=2)
			{	
			t_pix.color=0xff0000;
			t_pix.x = i;
			t_pix.y = 1;

			t_pix.xAcc = 0.0;
			t_pix.xVel = 0.0;
					
			t_pix.yAcc =0; //(t_pix.y-yCent)*409+rand()%3000-rand()%3000;
			t_pix.yVel = 100;
		
			t_pix.dimFactor = 0;
			createPixElement(&t_pix);	
		
			}
		  for(int i = 0; i<HEIGHT ; i+=2)
			{
			t_pix.color = 0x00ff00;
			t_pix.x = 1;
			t_pix.y =i;
			
			t_pix.xAcc = 0;
			t_pix.xVel = 100;

			t_pix.yAcc =0; //(t_pix.y-yCent)*409+rand()%3000-rand()%3000;
			t_pix.yVel = 0;
		
			t_pix.dimFactor = 0;
			createPixElement(&t_pix);	
		
			}
			int i =0;
			while(i++<900000);
			count++;
		}
	    break;
	   }
	   case 'e':
	   {
		t_pix.dimFactor = 40;		

		  for(theta = 0.0; theta<6.30 ; theta+=.1)
			{
			t_pix.x = xCent + cos(theta)*radius;
			t_pix.y = yCent + sin(theta)*radius;
		
			t_pix.xVel = (t_pix.x-xCent)*40+rand()%30-rand()%30;
			t_pix.yVel = (t_pix.y-yCent)*40+rand()%30-rand()%30;
		
			//t_pix.dimFactor = 9+rand()%30;
			createPixElement(&t_pix);	
		
		}

	    break;
	   }
	 

 }

}//END <runDemos>


//-------------RK4_parse-----------
//keeps running RK4_all (determines movement
//based on acceleration and such on all
//elements in pixel list), incrementing time 
//and rendering the pixels
void RuPPAT :: RK4_parse(SDL_Surface* background)
{
Uint32 T1=0, T2=1000;
T1=SDL_GetTicks(); 
float t = 0.0;
float dt = 0.004;

int engine_rate = 180;
int interval = 1000/engine_rate;
int nextTick = SDL_GetTicks() + interval;

char sel = 'g';
void * select = &sel;


	//keep looping until program end
	while(!done)
	  {

		RK4_all(t, dt);
		t += dt;
		
		mainRender->applySurface(0,0,backgroundLayers[0]);

		mainRender->applySurface(0,0,backgroundLayers[1]);


		//parse objects	
		parseObjectsToSurface();	
		
		//parse pixels
		parseSelectPixToSurface();

		//parse players
		parsePlayersToSurface();

		mainRender->applySurface(0,0,backgroundLayers[2]);

	
	mainRender->OnRender();
	
	if(nextTick > SDL_GetTicks())SDL_Delay(nextTick - SDL_GetTicks());
	
	nextTick = SDL_GetTicks() + interval;
	

	  }
	cout<<"Render thread ending!"<<endl;
}//END <RK4_parse>


//||||||||||||||||||||||||||||||||||||\\
//-------------------------------------\\
//	RuPPAT Entry Point
//spawn threads to do work
//
//
void RuPPAT :: runPPAT(bool *mainDone, Event_desc &mainEvents
			, string selection, string option)
{
bool *DONE = (bool*)mainDone;

//initPixLock();
//pthread_t computePhys_th; //does the number crunching for baisc physics
//pthread_t applyPhys_th;//constantly applies the numbers crunched by computPhys_th
//pthread_t demo_th;
pthread_t rk4_th;

//pthread_mutex_init(&pix_list_lock_2,NULL);

//launch computePhys thread into *calcGravFromPointsOnSelect()*
//pthread_create(&computePhys_th, NULL, pointsOnSelect_helper, this);

//launch applyPhys thread into updateSelectPix
//pthread_create(&applyPhys_th, NULL, updateSelectPix_helper, this );

cout<<"SELECTION: "<<selection<<"\t OPTION: "<<option<<endl;

char select;
	
	//set the demo selection
	if(selection == "gwell")select='g';
	else if(selection == "explode")select = 'e';
	else if(selection == "boxin")select = 'r';
	else select='r';

	//add any initial point masses pMassList or pixels here to pixelList
	Mass_desc tmp_mass = *(new Mass_desc);
		tmp_mass.xLoc = 612;
		tmp_mass.yLoc = 384;
		tmp_mass.mass = 400000;

	//atomically push mass to vector of masses
//	pthread_rwlock_wrlock(&mass_rw_lock);
//		pMassList.push_back(tmp_mass);
//	pthread_rwlock_unlock(&mass_rw_lock);

		tmp_mass.xLoc = 312;
		tmp_mass.yLoc = 284;
		tmp_mass.mass = 10000;

//		pMassList.push_back(tmp_mass);


	//prepare helper struct (NOT USED?)
	helper_char demo;
	demo.context = this;
	demo.character = &select;
		void* args = &demo;

	RK4_parse_helper_arg RK_help;
	//RK_help.backg=bkg;
	RK_help.context=this;
	void* RK_args = &RK_help;
	pthread_create(&rk4_th, NULL, RK4_parse_helper, RK_args);
	//pthread_create(&demo_th,NULL, runDemos_helper,args);

void *sel = &select;

	//go ahead and run demo on launch
	runDemos(sel);

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
		if(mainEvents.space && keyT1>60)
			{
			keyT1 = 0;
			cout<<"SPACE PRESSED!!!"<<endl;	

			//then run demo->place pixels w/ attr
			runDemos(sel);
			}
		//increment keypress counter
		keyT1++;

		if(nextTick > SDL_GetTicks())SDL_Delay(nextTick - SDL_GetTicks());
	
	nextTick = SDL_GetTicks() + interval;	

	}//END while

	//tell threads to finish
	done = true;

	//join on RK4_parse thread
	pthread_join(rk4_th, NULL);
}


//-----------------createPixElemenent-------
//create a pix descriptor from arguments and
//then push it to the pixel list vector
Pixel_desc tmpPix;	
void RuPPAT :: createPixElement(  int x, int y,
				int color,
				int xAccel,
				int yAccel,
				int accLength,
				int dimFactor,
				int mass )
{	
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
		tmpPix.xGacc = 0;
		tmpPix.yGacc = 0;

		tmpPix.Xtimer = thisTime;
		tmpPix.Ytimer = thisTime;

		tmpPix.deleteMe = false;
		tmpPix.ID=IDbase++;

		//set accellength:number of times to apply this accel to vel
		tmpPix.accelLength = accLength;
		tmpPix.updated = 0;

		//set dimfactor:higher=pixel fades away quicker
		tmpPix.dimFactor = dimFactor;

		    //finally, push tmp to vector
		  pthread_mutex_lock(&pix_list_lock_2);
		    pixelList_m.push_back(tmpPix);
		 pthread_mutex_unlock(&pix_list_lock_2);
}



void RuPPAT :: putPixel(int x, int y, unsigned int color, int id)
{
mainRender->putPixel(x,y, color, id);
}

void RuPPAT :: putSprite(int x, int y, SDL_Surface *sprite)
{
	mainRender->putSprite(x, y,sprite);

}

//----these below should be replaced by a better system
//for handling player control
void RuPPAT :: accelPlayer(int p_ID, bool isForward)
{
	pthread_rwlock_wrlock(&player_rw_lock);
		players[p_ID]->setAccelVectors(isForward);
	pthread_rwlock_unlock(&player_rw_lock);

	Pixel_desc t_pix;
		t_pix.color=0xaf0000;
		t_pix.xAcc = 0;
		t_pix.yAcc = 0;
		t_pix.accelLength = 2;
		t_pix.dimFactor = 10;

		t_pix.dimTimer = 0;
		t_pix.Xtimer =t_pix.Ytimer =0;// SDL_GetTicks();

		t_pix.deleteMe = false;

		t_pix.updated = 0;


	pthread_rwlock_wrlock(&player_rw_lock);


		players[p_ID]->setAccelVectors(isForward);
	
	//	t_pix.yVel = -1*(players[p_ID]->getYvel());
	//	t_pix.xVel = -1*(players[p_ID]->getXvel());
		players[p_ID]->getXY_exhaust(t_pix.xVel, t_pix.yVel);

	pthread_rwlock_unlock(&player_rw_lock);
	
	float origXvel=t_pix.xVel, origYvel=t_pix.yVel;
		cout<<origXvel<<"  "<<origYvel<<endl;	
	for(int i =0; i< 30; i++)
	{	
		t_pix.color += 0x000300;
		t_pix.xVel =(origXvel*60)+rand()%20 -rand()%20;
		t_pix.yVel =(origYvel*60)+rand()%20 - rand()%20;
		t_pix.x=players[p_ID]->getX()+rand()%2-rand()%2;
		t_pix.y=players[p_ID]->getY()+rand()%2-rand()%2;
		t_pix.x +=origXvel*10;
		t_pix.y +=origYvel*10;
	createPixElement(&t_pix);
	}
}

void RuPPAT :: turnPlayer(int p_ID, bool isLeft, int numTurns)
{

	while(numTurns-- > 0)
	{
		if(isLeft)
		{
			
			pthread_rwlock_wrlock(&player_rw_lock);
				players[p_ID]->incrementRotation_rate();
			pthread_rwlock_unlock(&player_rw_lock);
		}
		else
		{
			pthread_rwlock_wrlock(&player_rw_lock);
				players[p_ID]->decrementRotation_rate();
			pthread_rwlock_unlock(&player_rw_lock);
		}
	}

}

//This function is intended to work like a PID loop (proportional, integral, derivative)
// 	->the current implementation is a basterdize, semi-working version
// The goal is to continually adjust the turn amount until the ship is pointing towards
// the desired coordinates. Thus the manipulated variable is the rotation rate, and the
// set point is a heading or vector alignment
void RuPPAT :: turnPlayerToCoord(int p_ID, int x, int y, int rate)
{

	float playerDegree = 0.0,deg_playerToPoint = 0.0, 
		tempErrAccum, tempErrDiff, angleDifScale=.01;
	int x_diff = 0,  y_diff=0, rateLim = 3;

		//get some values out of the player object	
		pthread_rwlock_wrlock(&player_rw_lock);
	
			playerDegree = players[p_ID]->getAngle();
	
			tempErrAccum = players[p_ID]->rotationalErrorAccum;
			tempErrDiff = players[p_ID]->lastErr;

			x_diff = x - players[p_ID]->getX();
			y_diff = y - players[p_ID]->getY();
	
		pthread_rwlock_unlock(&player_rw_lock);

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

	pthread_rwlock_wrlock(&player_rw_lock);
		float curr_rate = players[p_ID]->getRotation_rate(); 
	pthread_rwlock_unlock(&player_rw_lock);

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
		pthread_rwlock_wrlock(&player_rw_lock);

		players[p_ID]->setRotation_rate(0);
	
		pthread_rwlock_unlock(&player_rw_lock);

	}
		pthread_rwlock_wrlock(&player_rw_lock);
	
//			playerDegree = players[p_ID]->getAngle();
	
			players[p_ID]->rotationalErrorAccum = tempErrAccum;
			players[p_ID]->lastErr = tempErrDiff;

	//		x_diff = x - players[p_ID]->getX();
	//		y_diff = y - players[p_ID]->getY();
	
		pthread_rwlock_unlock(&player_rw_lock);
printf("exiting!\n");
}


