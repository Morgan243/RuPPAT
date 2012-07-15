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

//cli options, defaults set here
	string selection = "gwell";
	string option;

	int WIDTH_cl = 1024;
	int HEIGHT_cl = 768;


//let X display know we are going to have multiple threads
int x_o = XInitThreads();

//stack is too small for this honey
//most init outside main to use heap instead
Game *game;

//funciton handles command line arguments
int handleInput(int argc,char *argv[]);



/*\\\\\\\\\\\\\\\\\\\\\\\\\
 * ||||||||||||||||||||||||
 * ENTRY POINT
 */
int main(int argc, char *argv[])
{
	//check that XInitThreads was not erroneous
	if(!x_o)
		cout<<"ERROR WITH XINIT THREADS!"<<endl;

	//handle command line args, exit if -help used
	if(handleInput(argc,argv)){return 0;}

	game = new Game(WIDTH_cl, HEIGHT_cl);
	
	//start up the game!!
	game->run(selection, option);

	return 0;
}

//handleInput assigns global variables based on the comman line options selected
int handleInput(int argc,char *argv[])
{
	int i,j,k=0;
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
	//	filename = argv[i+1];	
		}

		if(strcmp(argv[i],"-height")==0)
		{
			HEIGHT_cl=atoi(argv[i+1]);

		}	

		if(strcmp(argv[i],"-width")==0)
		{
			WIDTH_cl=atoi(argv[i+1]);

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
