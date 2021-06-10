#ifndef GAME_H
#define GAME_H
#include "include/SDL2/SDL.h"

class Game
{
public:
virtual bool Update(float deltaTime) = 0;
virtual void Render(SDL_Renderer* aRenderer) = 0;
static int WindowWidth;
static int WindowHeight;
};

#endif // GAME_H