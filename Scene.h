#ifndef SCENE_H
#define SCENE_H

#include "framework/include/SDL2/SDL.h"

class Scene {
public:
    virtual bool Update(float deltaTime) = 0;
    virtual void Render(SDL_Renderer* aRenderer) = 0;
    bool isFinished() {return myFinished; };
protected:
    bool myFinished;
};

#endif // SCENE_H