#ifndef DEMO_H
#define DEMO_H

#include "framework/Game.h"

class Demo : public Game
{
public:
    Demo();
    bool Update(float deltaTime);
    void Render(SDL_Renderer* aRenderer);
private:
    SDL_Texture* myLogo;
};

#endif // DEMO_H