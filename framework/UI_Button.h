#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "UI_Element.h"
#include "include/SDL2/SDL_ttf.h"

class UI_Button : public UI_Element {
public:
    // background image only
    UI_Button(const char* texturePath, SDL_Renderer* aRenderer, float xPos, float yPos);
    UI_Button(const char* texturePath, SDL_Renderer* aRenderer, SDL_Rect aPosition, ASPECT_RATIO ratio);
    // text only
    UI_Button(const char* text, TTF_Font* aFont, SDL_Color aFontColour, SDL_Renderer* aRenderer, float xPos, float yPos);
    UI_Button(const char* text, TTF_Font* aFont, SDL_Color aFontColour, SDL_Renderer* aRenderer, SDL_Rect aPosition, ASPECT_RATIO ratio);
    // both
    UI_Button(const char* texturePath, const char* text, TTF_Font* aFont, SDL_Color aFontColour, SDL_Renderer* aRenderer, float xPos, float yPos);
    UI_Button(const char* texturePath, const char* text, TTF_Font* aFont, SDL_Color aFontColour, SDL_Renderer* aRenderer, SDL_Rect aPosition, ASPECT_RATIO ratio);

    ~UI_Button();
    void Update(float deltaTime) override;
    void Render(SDL_Renderer* aRenderer) override;
    bool isClicked(){ return myClicked; };
protected:
    SDL_Texture* myText;
    SDL_Rect myTextPosition;
    bool myClicked;
    bool myHovering;
};

#endif // UI_BUTTON_H