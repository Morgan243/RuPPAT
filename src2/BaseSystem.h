#ifndef BASE_SYSTEM_H
#define BASE_SYSTEM_H

#include "SDL2/SDL.h"
//#include "SDL2/SDL_gfxPrimitives.h"
#include "SDL2/SDL2_rotozoom.h"
#include "SDL2/SDL_image.h"
#include <vector>

#include "Renderer.h"
#include "BaseEntity.h"

class BaseSystem
{
    private:
        Renderer *renderer;

    public:
        BaseSystem(Renderer *renderer);
        int update(std::vector<Entity> entities);
};

#endif
