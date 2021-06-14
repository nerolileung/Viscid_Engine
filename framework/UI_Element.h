#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_image.h"

class UI_Element {
public:
    UI_Element();
    UI_Element(const char* texturePath, SDL_Renderer* aRenderer);
    UI_Element(const char* texturePath, SDL_Renderer* aRenderer, float xPos, float yPos);
    ~UI_Element();
    virtual void Update(float deltaTime);
    virtual void Render(SDL_Renderer* aRenderer);
protected:
    SDL_Texture* myTexture;
    SDL_Rect myPosition;
};

#endif // UI_ELEMENT_H