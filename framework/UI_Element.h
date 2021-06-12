#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_image.h"

class UI_Element {
public:
    virtual void Update(float deltaTime);
    virtual void Render(SDL_Renderer* aRenderer);
protected:
    SDL_Texture* myTexture;
};

#endif // UI_ELEMENT_H