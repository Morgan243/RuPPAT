#include "math.h"
#include <iostream>
#include <iomanip>

#include "SpriteComponent.h"

using namespace std;
SpriteComponent::SpriteComponent(string sprite_path)
{
    SDL_Surface *tempSprite;

    tempSprite = IMG_Load((char *)sprite_path.c_str());

    base_sprite = tempSprite;

    rotations.push_back(
            rotozoomSurface(tempSprite,
                                    0.0,
                                    1.0,1));

}
