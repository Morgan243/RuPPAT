//INCLUDES
//{{{
#include <stdlib.h>
#include <fstream>
#include <string>
#include <math.h>
#include <iostream>
#include <omp.h>
#include <X11/Xlib.h>
#include <sys/resource.h>

//SDL Dependencies
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL2_rotozoom.h"
#include "SDL2/SDL_image.h"

#include "LuaParser.h"

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
//}}}

#pragma comment(lib, "lua.lib")
#pragma comment(lib, "lualib.lib")

using namespace std;

    //Basic globals
    //{{{
    int debug_level = 1;

	//depreciated way of doing options below
	string selection = "gwell";
	string option;

	//Default resolution
	int WIDTH_cl = 1024;
	int HEIGHT_cl = 768;

    //default config file name
    string filename = "config";
    //}}}


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

void readConfigLuaScript(string filename, vector<section> &configSection);

void readConfigMain(LuaParser &lua_parser, vector<section> &configSection);

void readConfigPlayers(LuaParser &lua_parser, 
                        vector<section> &configSection,
                        section &temp_section);

void readConfigMissiles(LuaParser &lua_parser,
                        vector<section> &configSection,
                        section &temp_section,
                        int player_num);

void readConfigObjects(LuaParser &lua_parser, 
                        vector<section> &configSection,
                        section &temp_section);


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

    //init runtime option struct
	RunOptions cmdLineOptions;

    //default config file path
    cmdLineOptions.config_path = "config";

	//handle command line args, exit if -help used
	if(handleInput(argc,argv,cmdLineOptions)){return 0;}

	vector<section> configSection, configSection_lua;

	//get configuration from file
	readConfigFile(filename, configSection);

    //experimental, read config from lua
    readConfigLuaScript("init_script.lua", configSection_lua);

    //cout<<Common::createReadableStringFromSection(configSection_lua)<<endl;

    //create the game object
	//game = new Game(configSection);
	game = new Game(configSection_lua);
	
	//start up the game!!
	game->run(selection, option);

    //lua_close(pL);

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

void readConfigLuaScript(string filename, vector<section> &configSection)
//{{{
{
    int num_players;
    section temp_section;
    optionSet temp_option;

    //load up a Lua parser
    LuaParser lua_parser(filename.c_str());

    readConfigMain(lua_parser, configSection);
}
//}}}

void readConfigMain(LuaParser &lua_parser, vector<section> &configSection)
//{{{
{
    int num_players;
    section temp_section;
    optionSet temp_option;
    
    //---Get "main" table---
    temp_section.title = "Main";

        temp_option.option = "width";
            temp_option.values.
                push_back(std::to_string(lua_parser.PopLuaTableIntegerValue("main", "width")));

                if(debug_level > 0)
                {
                    cout<<"\t+Option: "<<temp_option.option<<endl;
                    cout<<"\t++  "<< temp_option.values.back() <<endl;
                }

            //push back width option for the main set
            temp_section.sectionOptions.push_back(temp_option);
            //clear the temp values
            temp_option.values.clear();

        temp_option.option = "height";
            temp_option.values.
                push_back(std::to_string(lua_parser.PopLuaTableIntegerValue("main", "height")));

                if(debug_level > 0)
                {
                    cout<<"\t+Option: "<<temp_option.option<<endl;
                    cout<<"\t++  "<< temp_option.values.back() <<endl;
                }
             
            //height option for the main set
            temp_section.sectionOptions.push_back(temp_option);
            temp_option.values.clear();

        temp_option.option = "font";
            temp_option.values.
                push_back(lua_parser.PopLuaTableStringValue("main", "font"));

                if(debug_level > 0)
                {
                    cout<<"\t+Option: "<<temp_option.option<<endl;
                    cout<<"\t++  "<< temp_option.values.back() <<endl;
                }
             
            //height option for the main set
            temp_section.sectionOptions.push_back(temp_option);
            temp_option.values.clear();



            lua_getfield(lua_parser.GetState(), -1, "backgrounds");
            lua_getfield(lua_parser.GetState(), -1, "amount");
            int num_backgrounds = lua_tonumber(lua_parser.GetState(), -1);
            lua_pop(lua_parser.GetState(),1);

            temp_option.option = "bkg amount";
            temp_option.values.push_back(std::to_string(num_backgrounds));

            temp_section.sectionOptions.push_back(temp_option);
            temp_option.values.clear();
            cout<<"\t+ Number of background layers: " << num_backgrounds<<endl;

            //vector of the background sprites
            temp_option.option = "backgroundLayers" ;

            for(int i = 0; i < num_backgrounds; i++)
            {
                lua_rawgeti(lua_parser.GetState(), -1, i);
                string bg_sprite = lua_tostring(lua_parser.GetState(), -1);
                lua_pop(lua_parser.GetState(),1);
                cout<<"\t+Background layer " <<i<<": "<<bg_sprite<<endl;

                temp_option.values.push_back(bg_sprite);
            }

            temp_section.sectionOptions.push_back(temp_option);
            temp_option.values.clear();

            temp_option.option = "level style";
                temp_option.values.push_back(lua_parser.PopLuaTableStringValue("main", "level style"));
                cout<<"\t+Level Style: "<< temp_option.values.back()<<endl;
            temp_section.sectionOptions.push_back(temp_option);
            temp_option.values.clear();
            
        configSection.push_back(temp_section);
        temp_section.sectionOptions.clear();
 
        readConfigPlayers(lua_parser, configSection, temp_section);
        readConfigObjects(lua_parser, configSection, temp_section);
}
//}}}

void readConfigPlayers(LuaParser &lua_parser,
                       vector<section> &configSection,
                       section &temp_section)
 //{{{
{
    cout<<"##Reading Players##"<<endl;

    temp_section.title = "Player";
    optionSet temp_option;
    temp_option.option = "num players";

        //number of players to look for
        temp_option.values.
            push_back(std::to_string(lua_parser.PopLuaTableIntegerValue("players", "amount")));

        int num_players = std:: atoi( temp_option.values.back().c_str() );

        if(debug_level > 0)
            cout<<"\t+Number of players in config: " << num_players <<endl;

        temp_section.sectionOptions.push_back(temp_option);
        temp_option.values.clear();

    //add the main section to the configuration
    //configSection.push_back(temp_section);


    //some temp vars
    string sprite_name, hc_name;
    int x_pos, y_pos;

    configSection.push_back(temp_section);

    //should still have the player table on top of stack from the num_players loading
    if(lua_istable(lua_parser.GetState(), -1))
    {
        //cout<<"\t+got player table!"<<endl;

        //go through all the players
        for(int i = 0; i < num_players; i++)
        {
            //cout<<"\t+Parsing player table "<< i <<endl;
            //get the first player (index zero of players table in lua)
            lua_rawgeti(lua_parser.GetState(), -1, i);

            temp_section.sectionOptions.clear();
            temp_section.title = "player-" + std::to_string(i);
            cout<<"\t++player "<<i<<"++"<<endl;

            //make sure top item is a table
            if(lua_istable(lua_parser.GetState(), -1))
            {
                //cout<<"\t+got player zero table..!"<<endl;
                
                //get lua to put the current tables "sprite" key's value
                lua_getfield(lua_parser.GetState(), -1, "sprite");

                //grab the value of "sprite" from the first player's table
                sprite_name = lua_tostring(lua_parser.GetState(), -1);
                cout<<"\t+Sprite: "<< sprite_name << endl;
                
                //fill in options structure
                temp_option.option = "sprite";
                temp_option.values.push_back(sprite_name);
                temp_section.sectionOptions.push_back(temp_option);
                temp_option.values.clear();

                //pop the last value off, not needed anymore
                lua_pop(lua_parser.GetState(),1);

    
                //put current player table "x" value on stack
                lua_getfield(lua_parser.GetState(), -1, "x");

                //get a number off the top
                x_pos = lua_tonumber(lua_parser.GetState(), -1);
                cout<<"\t+X Pos: "<< x_pos<<endl;

                //fill in options structure
                temp_option.option = "x";
                temp_option.values.push_back(std::to_string(x_pos));
                temp_section.sectionOptions.push_back(temp_option);
                temp_option.values.clear();

                //pop the last value off, not needed anymore
                lua_pop(lua_parser.GetState(),1);


                //put current player table "x" value on stack
                lua_getfield(lua_parser.GetState(), -1, "y");

                //get a number off the top
                y_pos = lua_tonumber(lua_parser.GetState(), -1);
                cout<<"\t+Y Pos: "<< y_pos<<endl;
                
                //fill in options structure
                temp_option.option = "y";
                temp_option.values.push_back(std::to_string(y_pos));
                temp_section.sectionOptions.push_back(temp_option);
                temp_option.values.clear();

                //pop the last value off, not needed anymore
                lua_pop(lua_parser.GetState(),1);


                //put current player table "x" value on stack
                lua_getfield(lua_parser.GetState(), -1, "HC");
                
                //get a number off the top
                hc_name = lua_tostring(lua_parser.GetState(), -1);
                cout<<"\t+HC: "<< hc_name<<endl;

                //fill in options structure
                temp_option.option = "HC";
                temp_option.values.push_back(hc_name);
                temp_section.sectionOptions.push_back(temp_option);
                temp_option.values.clear();

                //pop the last value off, not needed anymore
                lua_pop(lua_parser.GetState(),1);

                //put current player table "x" value on stack
                lua_getfield(lua_parser.GetState(), -1, "max accel");
                
                //get a number off the top
                 string maxAcc =
                     std::to_string(lua_tonumber(lua_parser.GetState(), -1));

                cout<<"\t+max accel: "<< maxAcc <<endl;

                //fill in options structure
                temp_option.option = "max accel";
                temp_option.values.push_back(maxAcc);
                temp_section.sectionOptions.push_back(temp_option);
                temp_option.values.clear();

                //pop the last value off, not needed anymore
                lua_pop(lua_parser.GetState(),1);

              readConfigMissiles(lua_parser, configSection, temp_section, i);

                configSection.push_back(temp_section);

            }
            else
            {
                cout<<"ERROR: Table "<< i << " does not exist or is invalid!"<<endl;
            }
        }
    }
}
 //}}}

void readConfigMissiles(LuaParser &lua_parser, 
                        vector<section> &configSection, 
                        section &temp_section,
                        int player_num)
//{{{
{
    cout<<"\t##Reading Player "<< player_num<<" Missiles##"<<endl;
    optionSet temp_option;
    int i = player_num; //because Im lazy
    //grab the missiles table from the player table

    lua_getfield(lua_parser.GetState(), -1, "missiles");

    if(lua_istable(lua_parser.GetState(), -1))
    {
        int amount;
        string missle_sprite;

        //get the amount field from missiles on top of stack
        lua_getfield(lua_parser.GetState(), -1, "amount");
        
        //parse the amount field
        amount = lua_tonumber(lua_parser.GetState(), -1);
        cout<<"\t+Amount of missiles for player "<< i <<" : "<< amount << endl;

        //remove the last item
        lua_pop(lua_parser.GetState(), 1);

        //go through all the missiles in the missile table
        for(int j = 0; j < amount; j++)
        {
        //{{{
        
            //store the player that this missile goes to
            temp_option.option = "player id";
            temp_option.values.push_back(std::to_string(player_num));
            temp_section.sectionOptions.push_back(temp_option);

            cout<<"\n\t\t+Missile player id: "<< temp_option.values.back()<<endl;
            temp_option.values.clear();

            //get a missile table from missiles table onto the stack
            lua_rawgeti(lua_parser.GetState(), -1, j);

            //ensure it's a table
            if(lua_istable(lua_parser.GetState(), -1))
            {
                //get the sprite for this missile onto the top of the stack
                temp_option.values.push_back(lua_parser.GetStringFromField("sprite"));

                temp_option.option = "missile-" + std::to_string(j) + "-sprite";
                temp_section.sectionOptions.push_back(temp_option);
                cout<<"\t\t+Missile sprite: "<< temp_option.values.back()<<endl;
                temp_option.values.clear();


                //get the name for this missile 
                temp_option.values.push_back( lua_parser.GetStringFromField("name") );

                temp_option.option = "missile-" + std::to_string(j) + "-name";
                temp_section.sectionOptions.push_back(temp_option);
                cout<<"\t\t+Missile name: "<< temp_option.values.back()<<endl;
                temp_option.values.clear();
                
                
                //get the amount for this missile
                temp_option.values.push_back( lua_parser.GetStringFromField("amount") );

                temp_option.option = "missile-" + std::to_string(j) + "-amount";
                temp_section.sectionOptions.push_back(temp_option);
                cout<<"\t\t+Missile amount: "<< temp_option.values.back()<<endl;
                temp_option.values.clear();

                
                //get the damage for this missile 
                temp_option.values.push_back( lua_parser.GetStringFromField("damage") );

                temp_option.option = "missile-" + std::to_string(j) + "-damage";
                temp_section.sectionOptions.push_back(temp_option);
                cout<<"\t\t+Missile damage: "<< temp_option.values.back()<<endl;
                temp_option.values.clear();


                //get the velocity for this missile
                temp_option.values.push_back(lua_parser.GetStringFromField("velocity"));

                temp_option.option = "missile-" + std::to_string(j) + "-velocity";
                temp_section.sectionOptions.push_back(temp_option);
                cout<<"\t\t+Missile velocity: "<< temp_option.values.back()<<endl;
                temp_option.values.clear();


                //get the lifespan for this missile 
                temp_option.values.push_back(lua_parser.GetStringFromField("lifespan"));

                temp_option.option = "missile-" + std::to_string(j) + "-lifespan";
                temp_section.sectionOptions.push_back(temp_option);
                cout<<"\t\t+Missile lifespan: "<< temp_option.values.back()<<endl;
                temp_option.values.clear();

            }
            else
            {
                cout<<"Error with missile table!"<<endl;
            }
        //}}}
        }
    }
    else
    {
        cout<<"ERROR: Something wrong with player " 
            << i << "'s missiles table!"<<endl;
    }
}
//}}}

void readConfigObjects(LuaParser &lua_parser,
                        vector<section> &configSection,
                        section &temp_section)
//{{{
{
    optionSet temp_option;

    int num_objects = lua_parser.PopLuaTableIntegerValue("objects", "amount");

    cout<<"##Reading Objects##"<<endl;

    temp_section.title = "Objects";
    temp_section.sectionOptions.clear();

    //create the option
    temp_option.option = "num objects";
    temp_option.values.push_back(std::to_string(num_objects));
    temp_section.sectionOptions.push_back(temp_option);
    temp_option.values.clear();

    cout<<"\t+Num objects = "<<num_objects<<endl;

    configSection.push_back(temp_section);

    for(int i = 0; i < num_objects; i++)
    {
        temp_section.sectionOptions.clear();
        temp_section.title = "object " + std::to_string(i);
        

        //cout<<"\t+Parsing object table"<<endl;
        temp_option.option = "object id";
        temp_option.values.push_back(std::to_string(i));
        temp_section.sectionOptions.push_back(temp_option);
        cout<<endl;
        cout<<"\t+object id = " << temp_option.values.back() << endl;
        temp_option.values.clear();

        lua_rawgeti(lua_parser.GetState(), -1, i);

        if(lua_istable(lua_parser.GetState(), -1))
        {

            temp_option.option = "x";
            temp_option.values.push_back(std::to_string(lua_parser.GetIntegerFromField("x")));
            temp_section.sectionOptions.push_back(temp_option);
            cout<<"\t+x = "<<temp_option.values.back()<<endl;
            temp_option.values.clear();

            temp_option.option = "y";
            temp_option.values.push_back(std::to_string(lua_parser.GetIntegerFromField("y")));
            temp_section.sectionOptions.push_back(temp_option);
            cout<<"\t+y = "<<temp_option.values.back()<<endl;
            temp_option.values.clear();


            temp_option.option = "sprite";
            temp_option.values.push_back(lua_parser.GetStringFromField("sprite"));
            temp_section.sectionOptions.push_back(temp_option);
            cout<<"\t+sprite = "<<temp_option.values.back()<<endl;
            temp_option.values.clear();


            temp_option.option = "HC";
            temp_option.values.push_back(lua_parser.GetStringFromField("HC"));
            temp_section.sectionOptions.push_back(temp_option);
            cout<<"\t+HC = "<<temp_option.values.back()<<endl;
            temp_option.values.clear();


            temp_option.option = "mass";
            temp_option.values.push_back(std::to_string(lua_parser.GetIntegerFromField("mass")));
            temp_section.sectionOptions.push_back(temp_option);
            cout<<"\t+mass = "<<temp_option.values.back()<<endl;
            temp_option.values.clear();

            temp_option.option = "rotation";
            temp_option.values.
                push_back(std::to_string(lua_parser.GetIntegerFromField("rotation")));
            temp_section.sectionOptions.push_back(temp_option);
            cout<<"\t+rotation = "<<temp_option.values.back()<<endl;
            temp_option.values.clear();

            temp_option.option = "x velocity";
            temp_option.values.
                push_back(std::to_string(lua_parser.GetIntegerFromField("x velocity")));
            temp_section.sectionOptions.push_back(temp_option);
            cout<<"\t+x velocity = "<<temp_option.values.back()<<endl;
            temp_option.values.clear();

            temp_option.option = "y velocity";
            temp_option.values.
                push_back(std::to_string(lua_parser.GetIntegerFromField("y velocity")));
            temp_section.sectionOptions.push_back(temp_option);
            cout<<"\t+y velocity = "<<temp_option.values.back()<<endl;
            temp_option.values.clear();


            lua_pop(lua_parser.GetState(), 1);
        }
        configSection.push_back(temp_section);
    }
}
//}}}
