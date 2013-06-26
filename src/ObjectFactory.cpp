#include "ObjectFactory.h"

    //Static initializations
    int ObjectFactory::total_created;
    int ObjectFactory::pixelGroups_created;
    int ObjectFactory::objects_created;
    int ObjectFactory::AI_created;
    int ObjectFactory::players_created;

    unsigned int ObjectFactory::std_num_rotations;
    unsigned int ObjectFactory::std_start_angle;

std::vector<Pixel_desc> ObjectFactory::NewPixelGroup(const std::string &description,
                                                        const unsigned int size,
                                                        const int seed ,
                                                        const bool randomize )
{
    total_created++;
    pixelGroups_created++;

    if(description == "floaters")
    {

    }
    else if(description == "galaxy")
    {

    }
    else if(description == "explosion")
    {

    }
    else if(description == "stream")
    {

    }
}

Object* ObjectFactory::NewObject(const std::string &description,
                                    const std::string sprite_name)
{
    total_created++;
    objects_created++;

    if(description == "asteroid")
    {

    }
    else if (description == "planet")
    {

    }
}

Player* ObjectFactory::NewAI(const std::string &description, AI_Relation relation, AI_Skill skill)
{
    total_created++;
    AI_created++;
    
    if(description == "leech")
    {

    }
}

Player* ObjectFactory::NewPlayer(const std::string &description)
{
    total_created++;
    players_created++;

    Player *new_player;

    if(description == "wraith")
    {
        new_player = new Player("red_ship_wraith.png",
                                std_num_rotations,
                                std_start_angle,
                                 1.0,
                                 0,0,
                                 "red_ship_HC.png");
    }
}

