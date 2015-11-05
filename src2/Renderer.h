#ifndef RENDERER_H
#define RENDERER_H

#include "SDL2/SDL.h"
//#include "SDL2/SDL_gfxPrimitives.h"
#include "SDL2/SDL2_rotozoom.h"
#include "SDL2/SDL_image.h"
#include <vector>

class Renderer
{
    private:
        int resolution_width, resolution_height;

        SDL_Window      *main_window;
        SDL_Renderer    *renderer; //sdl uses its magic to make the pretties come on screen
        SDL_Texture     *main_texture; //streaming texture that we give to renderer when bliting

    public:
        Renderer(int resolution_weight, int resolution_height);

        void render(){SDL_RenderPresent(this->renderer);}
        void putSprite(const int x, const int y, SDL_Surface* sprite);

};

#endif
