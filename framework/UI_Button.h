#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "UI_Element.h"
#include "include/SDL2/SDL_ttf.h"

class UI_Button : public UI_Element {
public:
    // we always have a renderer and position
    // button may have image (inherits from ui_element) and/or text (doesn't), may or may not have sfx

    // render full text/image
    UI_Button(SDL_Renderer* aRenderer, float xPos, float yPos,
        const char* text = nullptr, TTF_Font* aFont = nullptr, SDL_Color aFontColour = {0,0,0,0},
        const char* clickSFXFile = nullptr, const char* hoverSFXFile = nullptr);
    UI_Button(const char* texturePath,
        SDL_Renderer* aRenderer, float xPos, float yPos,
        const char* text = nullptr, TTF_Font* aFont = nullptr, SDL_Color aFontColour = {0,0,0,0},
        const char* clickSFXFile = nullptr, const char* hoverSFXFile = nullptr);

    // resize text/image to fit given rectange/aspect ratio
    UI_Button(SDL_Renderer* aRenderer, SDL_Rect aPosition, ASPECT_RATIO ratio,
        const char* text = nullptr, TTF_Font* aFont = nullptr, SDL_Color aFontColour = {0,0,0,0},
        const char* clickSFXFile = nullptr, const char* hoverSFXFile = nullptr);
    UI_Button(const char* texturePath,
        SDL_Renderer* aRenderer, SDL_Rect aPosition, ASPECT_RATIO ratio,
        const char* text = nullptr, TTF_Font* aFont = nullptr, SDL_Color aFontColour = {0,0,0,0},
        const char* clickSFXFile = nullptr, const char* hoverSFXFile = nullptr);

    ~UI_Button();
    void Update(float deltaTime) override;
    void Render(SDL_Renderer* aRenderer) override;
    bool isClicked(){ return myClicked; };
protected:
    SDL_Texture* myText;
    SDL_Rect myTextPosition;
    bool myClicked;
    bool myHovering;
    int myHoverSFXID;
    int myClickSFXID;
};

#endif // UI_BUTTON_H