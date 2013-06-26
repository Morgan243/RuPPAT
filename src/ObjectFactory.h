#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H
#include "Player.h"
#include <vector>


class ObjectFactory
{
    private:
        static unsigned int std_num_rotations, std_start_angle;

    public:
        enum AI_Relation { enemy, apathetic, friendly, ally};
        enum AI_Skill {lifeless, easy, medium, hard };

        static int total_created,
                   pixelGroups_created,
                   objects_created,
                   AI_created,
                   players_created;

        //create differemt groups of pixels: floaters, galaxy, explosion, stream)
        static std::vector<Pixel_desc> NewPixelGroup(const std::string &description = "floaters",
                                                        const unsigned int size = 20,
                                                        const int seed = 0,
                                                        const bool randomize = false);

        //create objects: asteroid, planet
        static Object *NewObject(const std::string &description = "asteroid",
                                    const std::string sprite_name = "ANY");

        //create a new AI 
        static Player *NewAI(const std::string &description = "leech",
                                 AI_Relation relation = enemy,
                                 AI_Skill skill = medium);

        //creat players: user
        static Player *NewPlayer(const std::string &description = "wraith");

};

#endif
