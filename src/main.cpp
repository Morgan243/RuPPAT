#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <omp.h>
#include <X11/Xlib.h>

#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"

#include "Descriptors.h"
#include "Game.h"
#include "Render.h"

using namespace std;

//depreciated way od doing options below
//cli options, defaults set here
	string selection = "gwell";
	string option;

	//default resolution
	int WIDTH_cl = 1024;
	int HEIGHT_cl = 768;
/////////////////////////////////

//let X display know we are going to have multiple threads
//working on the buffer
int x_o = XInitThreads();

//stack is too small for this honey
//most init outside main to use heap instead
Game *game;

//handle command line arguments
int handleInput(int argc,char *argv[], RunOptions &cl_options);


/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 * ||||||||||||||||||||||||||||||||
 * `````````ENTRY POINT````````````
 */
int main(int argc, char *argv[])
{
	//check that XInitThreads was not erroneous
	if(!x_o)
		cout<<"ERROR WITH XINIT THREADS!"<<endl;

	//init runtime option struct
	RunOptions cmdLineOptions;

		//set default resolution
		cmdLineOptions.height = 768;
		cmdLineOptions.width = 1024;

		//set default background layer paths
		cmdLineOptions.background_spritePaths[0]
					="bkg_one_1080.png";
		cmdLineOptions.background_spritePaths[1]
					="bkg_two_1080.png";
		cmdLineOptions.background_spritePaths[2]
					="bkg_three_1080.png";

		//set default player sprit attribute
		cmdLineOptions.player_spritePath = "red_ship.png";
		cmdLineOptions.player_HCpath = "red_ship_HC.png";

		//load a few default objects
		cmdLineOptions.objects_spritePath.push_back
						("red_planet.png");
		cmdLineOptions.objects_HCpath.push_back
						("red_planet_HC.png");

		cmdLineOptions.objects_spritePath.push_back
						("asteroid_medium_1.png");
		cmdLineOptions.objects_HCpath.push_back
						("asteroid_medium_1_HC.png");
	
	//handle command line args, exit if -help used
	if(handleInput(argc,argv,cmdLineOptions)){return 0;}

	//construct a new game, this only initiates vars and such
	//game = new Game(WIDTH_cl, HEIGHT_cl);
	game = new Game(cmdLineOptions);
	
	//start up the game!!
	game->run(selection, option);

	return 0;
}

//handleInput assigns global variables based on the comman line options selected
int handleInput(int argc,char *argv[], RunOptions &cl_options)
{
	int i,j,k=0, num_objects = 0;
	for(i=1;i<argc;i++)
	{
		
	if((strcmp(argv[i],"-help")==0)||(strcmp(argv[i],"-h")==0))
		{
			cout << "\n\n\t______RuPAP Engine Demo______"<<endl;
			cout<<"\t\tMorgan Stuart\n"<< endl;
			cout << "  run gravity well demo: '-gwell'"<< endl;
			cout << "          run fire demo: '-fire'"<< endl;
			cout << "run particle orbit demo: '-porbit'"<< endl;

			cout << "                print this menu: '-help'\n\n"<< endl;
			k=1;
		}


	if((strcmp(argv[i],"-file")==0)||(strcmp(argv[i],"-f")==0))
		{
		//filename = argv[i+1];	
		}

		if(strcmp(argv[i],"-height")==0)
		{
			//HEIGHT_cl=atoi(argv[i+1]);
			cl_options.height = atoi(argv[i+1]);	
		}	

		if(strcmp(argv[i],"-width")==0)
		{
			//WIDTH_cl=atoi(argv[i+1]);
			cl_options.width = atoi(argv[i+1]);	

		}	
	
		if(strcmp(argv[i],"-background")==0)
		{
			cl_options.background_spritePaths[0]=(argv[i+1]);
			cl_options.background_spritePaths[1]=(argv[i+2]);
			cl_options.background_spritePaths[2]=(argv[i+3]);
		}

		if(strcmp(argv[i],"-object")==0)
		{
		
			if(num_objects<2)
			{
			cl_options.objects_spritePath[num_objects]=(argv[i+1]);

			cout<<"CL arg: "<<argv[i+1]<<endl;

			cl_options.objects_HCpath[num_objects] = (argv[i+2]);

			cout<<"CL arg: "<<argv[i+2]<<endl;
			}
			else
			{
			cl_options.objects_spritePath.push_back(argv[i+1]);

			cout<<"CL arg: "<<argv[i+1]<<endl;

			cl_options.objects_HCpath.push_back(argv[i+2]);

			cout<<"CL arg: "<<argv[i+2]<<endl;
			}
			num_objects++;
			
		}
	
		if(strcmp(argv[i],"-gwell")==0)
		{	
			selection = "gwell";
		}

		if(strcmp(argv[i],"-explode")==0)
		{	
			selection = "explode";
		}

		if(strcmp(argv[i],"-boxin")==0)
		{	
			selection = "boxin";
		}

		//unimplemented!	
		if(strcmp(argv[i],"-fire")==0)
		{
			selection = "fire";
		}

		if(strcmp(argv[i],"-porbit")==0)
		{
			selection = "porbit";
		}

		if(strcmp(argv[i],"-hose")==0)
		{
			selection = "hose";
		}

		if(strcmp(argv[i],"-explode")==0)
		{
			selection = "explode";
		}


		if(strcmp(argv[i],"-r")==0)
		{
		//		choice = 1;				
		}

		if(strcmp(argv[i],"-s")==0)
		{
		//		choice = 2;				
		}

		if(strcmp(argv[i],"-p")==0)
		{
		//		choice = 3;
		}


		if(strcmp(argv[i],"-threads")==0)
		{
		//	THREADS=atoi(argv[i+1]);

		}		
	}
	return k;
}
