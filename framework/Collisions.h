#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "include/SDL2/SDL.h"

class Collisions {
public:
    static bool Circle(SDL_FPoint midpointA, float radiusA, SDL_FPoint midpointB, float radiusB);
    static bool Box(SDL_Rect boxA, SDL_Rect boxB);
};

#endif // COLLISIONS_H