#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_image.h"

class UI_Element {
public:
    enum ASPECT_RATIO {
        NONE = 0,
        HEIGHT = 1,
        WIDTH = 2,
    };

    UI_Element();
    UI_Element(const char* texturePath, SDL_Renderer* aRenderer);
    // positions are centered on the midpoint of the element
    UI_Element(const char* texturePath, SDL_Renderer* aRenderer, float xPos, float yPos);
    UI_Element(const char* texturePath, SDL_Renderer* aRenderer, SDL_Rect aPosition, ASPECT_RATIO ratio);
    ~UI_Element();
    virtual void Update(float deltaTime);
    virtual void Render(SDL_Renderer* aRenderer);
    void SetPosition(SDL_Point aPosition);
    SDL_Rect GetDimensions() { return myPosition; };
protected:
    SDL_Texture* myTexture;
    SDL_Rect myPosition;
};

#endif // UI_ELEMENT_H