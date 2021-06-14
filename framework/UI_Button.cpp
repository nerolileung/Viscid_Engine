#include "UI_Button.h"

UI_Button::UI_Button(const char* texturePath, SDL_Renderer* aRenderer, float xPos, float yPos) : UI_Element(texturePath, aRenderer, xPos, yPos){
    myClicked = false;
    myHovering = false;
}

UI_Button::UI_Button(const char* texturePath, SDL_Renderer* aRenderer, SDL_Rect aPosition) : UI_Element(texturePath, aRenderer){
    myPosition = aPosition;
    myPosition.x = aPosition.x - (aPosition.w / 2);
    myPosition.y = aPosition.y - (aPosition.h / 2);
    myClicked = false;
    myHovering = false;
}

UI_Button::UI_Button(const char* text, TTF_Font* aFont, SDL_Color aFontColour, SDL_Renderer* aRenderer, float xPos, float yPos){
    SDL_Surface* tempSurface = TTF_RenderText_Blended(aFont,text,aFontColour);
    myText = SDL_CreateTextureFromSurface(aRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    SDL_QueryTexture(myText,NULL,NULL,&myPosition.w,&myPosition.h);
    myPosition.x = xPos - (myPosition.w / 2);
    myPosition.y = yPos - (myPosition.y / 2);
    myClicked = false;
    myHovering = false;
}

UI_Button::UI_Button(const char* texturePath, const char* text, TTF_Font* aFont, SDL_Color aFontColour, SDL_Renderer* aRenderer, float xPos, float yPos) : UI_Element(texturePath, aRenderer, xPos, yPos){
    SDL_Surface* tempSurface = TTF_RenderText_Blended(aFont,text,aFontColour);
    myText = SDL_CreateTextureFromSurface(aRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    myClicked = false;
    myHovering = false;
}

UI_Button::~UI_Button(){
    if (myTexture != nullptr){
        SDL_DestroyTexture(myTexture);
        myTexture = nullptr;
    }
    if (myText != nullptr){
        SDL_DestroyTexture(myText);
        myText = nullptr;
    }
}

void UI_Button::Update(float deltaTime){
    // todo check for collisions
    // todo set opacity based on hover state
    // todo play sound effect when clicked
}

void UI_Button::Render(SDL_Renderer* aRenderer){
    if (myTexture != nullptr)
        SDL_RenderCopy(aRenderer, myTexture, NULL, &myPosition);
    else if (myText != nullptr)
        SDL_RenderCopy(aRenderer, myText, NULL, &myPosition);

    if (myText != nullptr && myTexture != nullptr){
        SDL_Rect myTextPosition;
        SDL_QueryTexture(myText,NULL,NULL,&myTextPosition.w,&myTextPosition.h);
        myTextPosition.x = myPosition.x + (myPosition.w/2) - (myTextPosition.w/2);
        myTextPosition.y = myPosition.y + (myPosition.h/2) - (myTextPosition.h/2);
        SDL_RenderCopy(aRenderer, myText, NULL, &myTextPosition);
    }
}