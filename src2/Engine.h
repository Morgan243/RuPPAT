#ifndef ENGINE_H
#define ENGINE_H

#include "SDL2/SDL.h"
//#include "SDL2/SDL_gfxPrimitives.h"
#include "SDL2/SDL2_rotozoom.h"
#include "SDL2/SDL_image.h"
#include <vector>


#include "BaseSystem.h"
#include "Renderer.h"


class Engine
{
    private:
        //std::vector<System> active_systems;
        BaseSystem player_control;

        BaseSystem physics;

        Renderer renderer;

    public:
        Engine();

        // Returns index of system
        //int add_system(System system);

        int update();
};
#endif
