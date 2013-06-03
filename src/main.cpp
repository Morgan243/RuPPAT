#include <stdlib.h>
#include <fstream>
#include <string>
#include <math.h>
#include <iostream>
#include <omp.h>
#include <X11/Xlib.h>
#include <sys/resource.h>

//SDL Dependencies
#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"

//Demo Headers Included
#include "Descriptors.h"
#include "Game.h"

//Lua Scripting support
extern "C"
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

#pragma comment(lib, "lua.lib")
#pragma comment(lib, "lualib.lib")


using namespace std;

	//depreciated way of doing options below
	string selection = "gwell";
	string option;

	//Default resolution
	int WIDTH_cl = 1024;
	int HEIGHT_cl = 768;

//default config file name
string filename = "config";


//let X display know we are going to have multiple threads
//working on the buffer
int x_o = XInitThreads();

//stack is too small for this puppy
//most init outside main to use heap instead
Game *game;

//handle command line arguments
int handleInput(int argc,char *argv[], RunOptions &cl_options);

//scan configuration from config file
void readConfigFile(string filename, vector<section> &configSection); 

//init lua
lua_State* initLua();


/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 * ||||||||||||||||||||||||||||||||
 * `````````ENTRY POINT````````````
 * ||||||||||||||||||||||||||||||||
 */////////////////////////////////*/
int main(int argc, char *argv[])
{
//{{{
	//check that XInitThreads was not erroneous
	if(!x_o)
		cout<<"ERROR WITH XINIT THREADS!"<<endl;


	struct rlimit rl;

	if(!getrlimit(RLIMIT_STACK, &rl))
	{
		rl.rlim_cur = 16*1024*1024*4; //64MB

		if(setrlimit(RLIMIT_STACK, &rl))
			{
				fprintf(stderr, "setrlimit failed\n");
				printf("stack limit setting failed!\n");
			}	
	}

    lua_State *pL = initLua();

    luaL_dofile(pL, "init_script.lua");
	//init runtime option struct
	RunOptions cmdLineOptions;

    //default config file path
    cmdLineOptions.config_path = "config";

	//handle command line args, exit if -help used
	if(handleInput(argc,argv,cmdLineOptions)){return 0;}

	vector<section> configSection;

	//get configuration from file
	readConfigFile(filename, configSection);

    //create the game object
	game = new Game(configSection);
	
	//start up the game!!
	game->run(selection, option);

    lua_close(pL);

	return 0;
//}}}
}

//handleInput assigns global variables based on the command line options selected
int handleInput(int argc,char *argv[], RunOptions &cl_options)
{
	//{{{
	int i,j,k=0, num_objects = 0;
	for(i=1;i<argc;i++)
	{
		
	if((strcmp(argv[i],"-help")==0)
	   ||(strcmp(argv[i],"-h")==0) 
	   || (strcmp(argv[i],"--help")==0))
		{
            //{{{
			cout << "\n\n\t______RuPAP Engine Demo______"<<endl;
			cout<<"\t\tMorgan Stuart\n"<< endl;
			cout << "The granular CLI options have been deprecated."<< endl;
			cout << "They will not override the config. They do nothing."<< endl;
			cout << "Instead, use a config file. A default should have"<< endl;
			cout << "been included called \"config\". You can specify a "<<endl;
			cout << "different file using the config file option."<< endl;
			cout << "This is the only option you should be using.\n"<< endl;
			cout <<"-cd, --config-file \t Specify config file name\n"<<endl;
			cout << "                print this menu: '-help'\n\n"<< endl;
			k=1;
            //}}}
		}


	if((strcmp(argv[i],"--config-file")==0)||(strcmp(argv[i],"-cf")==0))
		{
		    cl_options.config_path = argv[i+1];	
		}

		if(strcmp(argv[i],"-height")==0)
		{
			//HEIGHT_cl=atoi(argv[i+1]);
			//cl_options.height = atoi(argv[i+1]);	
		}	

		if(strcmp(argv[i],"-width")==0)
		{
			//WIDTH_cl=atoi(argv[i+1]);
			//cl_options.width = atoi(argv[i+1]);	

		}	
	
		if(strcmp(argv[i],"-background")==0)
		{
			//cl_options.background_spritePaths[0]=(argv[i+1]);
			//cl_options.background_spritePaths[1]=(argv[i+2]);
			//cl_options.background_spritePaths[2]=(argv[i+3]);
		}

		if(strcmp(argv[i],"-object")==0)
		{
		
			if(num_objects<2)
			{
				//cl_options.objects_spritePath[num_objects]=(argv[i+1]);

				cout<<"CL arg: "<<argv[i+1]<<endl;

				//cl_options.objects_HCpath[num_objects] = (argv[i+2]);

				cout<<"CL arg: "<<argv[i+2]<<endl;
			}
			else
			{
				//cl_options.objects_spritePath.push_back(argv[i+1]);

				cout<<"CL arg: "<<argv[i+1]<<endl;

				//cl_options.objects_HCpath.push_back(argv[i+2]);

				cout<<"CL arg: "<<argv[i+2]<<endl;
			}

			num_objects++;
		}
	
		if(strcmp(argv[i],"-gwell")==0)
		{	
			//selection = "gwell";
		}

		
		if(strcmp(argv[i],"-explode")==0)
		{	
			//selection = "explode";
		}

		if(strcmp(argv[i],"-boxin")==0)
		{	
			//selection = "boxin";
		}

		//unimplemented!	
		if(strcmp(argv[i],"-fire")==0)
		{
			//selection = "fire";
		}

		if(strcmp(argv[i],"-porbit")==0)
		{
			//selection = "porbit";
		}

		if(strcmp(argv[i],"-hose")==0)
		{
			//selection = "hose";
		}

		if(strcmp(argv[i],"-explode")==0)
		{
			//selection = "explode";
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
//}}}
}

void readConfigFile(string filename, vector<section>& configSections)
{
//{{{
	int place = 0;
	string line;

	ifstream inFile;	
	inFile.open( filename.c_str() );

	//keep moving through file while not at end
	while(inFile.good())
	{		
		//get line from inFile into line
		getline( inFile , line );
		
		//find a hashtag that marks comments
		int commentIndex = line.find('#',0);

		place = line.find(':',0);

		//if this is not a section header line (ex: "main:")
		//and the comment hash doesnt exist or doesnt start the line (!=0)
		if( (place == -1) && (commentIndex != 0))
		{
			//not a new section, add a new option to current section
			//then add values to the new section

			//fint the '=', the start of option values
			place = line.find('=',0);

			//charater after '='
			int tempPlace = place+1;
		
			//make sure there are some values (not all commented out)	
			if(commentIndex > tempPlace || commentIndex == -1 && place != -1)
			{
  		  	    //go ahead and create new option
  		  	    optionSet nextOptionSet;
  		 
			    int stringStart=line.find('\t',0); 
			    if(stringStart)
  			  	    //get the option name out of line
	  		  	    nextOptionSet.option = line.substr
					(0,place);
			    else
				    nextOptionSet.option = line.substr
				    	(stringStart+1,place - stringStart-1);

  		  	    cout<<"Option: "<<nextOptionSet.option<<endl;

			    bool leaveOnComment = false;

  		  	    //go through line, finding ',', and setting the value 
  		  	    for(int i = place+1; i<line.length() && !leaveOnComment; i++)
  		  	    {
  		  	    	
				//if comma and not hash
  		  	    	if(line[i] == ',' && line[i] != '#')
  		  	    	{
  		  	    		//add new value: string from last(tempPlace) to i
  		  	    		nextOptionSet.values.push_back
  		  	    			(line.substr(tempPlace,i - tempPlace));
  		  	    			
  		  	    		cout<<"New value: "
  		  	    			<<nextOptionSet.values.back()
  		  	    			<<endl;
  
  		  	    			//set temp place to char after ','
  		  	    			tempPlace = i+1;
  		  	    	}	
				else if(line[i] == '#')//found comment, break out of loop
				{
					leaveOnComment = true;
				}
  		  	    }
  	
  	  	  //there will always be one value not contained between two delimiters
  	  	  //ex: backgroundLayers=bkg_one_1080.png,bkg_two_1080.png,bkg_three_1
  		  	   if(!leaveOnComment)
			   {
				    nextOptionSet.values.push_back
					(line.substr(tempPlace,line.length() - tempPlace));
			   }
			   else
			   {
			   	nextOptionSet.values.push_back
					(line.substr(tempPlace, commentIndex - tempPlace));
			   }
  		  	    cout<<"New value: "<<nextOptionSet.values.back()<<endl;
  		  
  		  	    //finally add the option into the section's fields
  		  	    configSections.back().sectionOptions.push_back(nextOptionSet);
			}

		}
		//it is a new section, make new section struct
		else if (commentIndex !=0 && 
				((commentIndex > place) || commentIndex < 0))
		{
			//declare new section
			section nextSection;

			//get the title out of line (string up to ':')
			nextSection.title = line.substr(0,place);

			cout<<"Section Title: "<<nextSection.title<<endl;
			
			//add the new section
			configSections.push_back(nextSection);
		}
	}
//}}}
}


lua_State* initLua()
{

    //create Lua state
    lua_State *pL = luaL_newstate();

    //open all standard Lua libraries
    luaL_openlibs(pL);

    return pL;
}
