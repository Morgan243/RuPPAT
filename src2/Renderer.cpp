#include "Renderer.h"


Renderer::Renderer(int resolution_width,
                   int resolution_height){
    this->resolution_width = resolution_width;
    this->resolution_height = resolution_height;
    this->main_window = SDL_CreateWindow("RuPPAT Demo",
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      resolution_width, resolution_height,
                                      0);

    //associate the main window with a new renderer
    this->renderer = SDL_CreateRenderer(this->main_window, -1,
                                        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->renderer);
    SDL_RenderPresent(this->renderer);
}

void Renderer::putSprite(const int x, const int y, SDL_Surface* sprite){
    SDL_Rect imagePosition;

    imagePosition.x = x - (sprite->w/2);
    imagePosition.y = y - (sprite->h/2);

    imagePosition.w = sprite->w;
    imagePosition.h = sprite->h;

    //if(SDL_MUSTLOCK(pre_surface))
    //  SDL_LockSurface(pre_surface);

        //finally blit the source onto main at (x,y) on main
        //SDL_BlitSurface(sprite,NULL,pre_surface,&imagePosition);
        SDL_UpdateTexture(this->main_texture, &imagePosition,
                          sprite->pixels, sprite->pitch);

    //if(SDL_MUSTLOCK(pre_surface))
    //  SDL_UnlockSurface(pre_surface);
}


