#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL2_rotozoom.h"
#include "SDL/SDL_image.h"

#include "BaseComponent.h"
#include <iostream>
#include <string>
#include <vector>

class SpriteComponent: public Component
{
    private:
        SDL_Surface *base_sprite;
        std::vector<SDL_Surface*> rotations;

    public:
        SpriteComponent(std::string sprite_path);

};
