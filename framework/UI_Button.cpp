#include "UI_Button.h"
#include "AudioSystem.h"

UI_Button::UI_Button(SDL_Renderer* aRenderer, float xPos, float yPos, const char* text, TTF_Font* aFont, SDL_Color aFontColour, const char* clickSFXFile, const char* hoverSFXFile){
    SDL_Surface* tempSurface = TTF_RenderText_Blended(aFont,text,aFontColour);
    myText = SDL_CreateTextureFromSurface(aRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    SDL_QueryTexture(myText,NULL,NULL,&myPosition.w,&myPosition.h);
    myPosition.x = xPos - (myPosition.w / 2);
    myPosition.y = yPos - (myPosition.h / 2);

    myClicked = false;
    myHovering = false;
    if (clickSFXFile != nullptr)
        myClickSFXID = AudioSystem::LoadClip(clickSFXFile);
    else myClickSFXID = -1;
    if (hoverSFXFile != nullptr)
        myHoverSFXID = AudioSystem::LoadClip(hoverSFXFile);

    SDL_SetTextureAlphaMod(myText, 230);
}

UI_Button::UI_Button(SDL_Renderer* aRenderer, SDL_Rect aPosition, ASPECT_RATIO ratio, const char* text, TTF_Font* aFont, SDL_Color aFontColour, const char* clickSFXFile, const char* hoverSFXFile){
    SDL_Surface* tempSurface = TTF_RenderText_Blended(aFont,text,aFontColour);
    myText = SDL_CreateTextureFromSurface(aRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    SDL_QueryTexture(myText,NULL,NULL,&myPosition.w,&myPosition.h);
    switch (ratio){
        case ASPECT_RATIO::NONE:
            myPosition = aPosition;
        break;
        case ASPECT_RATIO::HEIGHT:
            myPosition.w *= aPosition.h/myPosition.h;
            myPosition.h = aPosition.h;
        break;
        case ASPECT_RATIO::WIDTH:
            myPosition.h *= aPosition.w/myPosition.w;
            myPosition.w = aPosition.w;
        break;
    }
    myPosition.x = aPosition.x - (myPosition.w / 2);
    myPosition.y = aPosition.y - (myPosition.h / 2);

    myClicked = false;
    myHovering = false;
    if (clickSFXFile != nullptr)
        myClickSFXID = AudioSystem::LoadClip(clickSFXFile);
    else myClickSFXID = -1;
    if (hoverSFXFile != nullptr)
        myHoverSFXID = AudioSystem::LoadClip(hoverSFXFile);

    SDL_SetTextureAlphaMod(myText, 230);
}

UI_Button::UI_Button(const char* texturePath, SDL_Renderer* aRenderer, float xPos, float yPos, const char* text, TTF_Font* aFont, SDL_Color aFontColour, const char* clickSFXFile, const char* hoverSFXFile) : UI_Element(texturePath, aRenderer, xPos, yPos){
    if (text != nullptr){
        SDL_Surface* tempSurface = TTF_RenderText_Blended(aFont,text,aFontColour);
        myText = SDL_CreateTextureFromSurface(aRenderer, tempSurface);
        SDL_FreeSurface(tempSurface);

        SDL_QueryTexture(myText,NULL,NULL,&myTextPosition.w,&myTextPosition.h);
        myTextPosition.x = myPosition.x + (myPosition.w/2) - (myTextPosition.w/2);
        myTextPosition.y = myPosition.y + (myPosition.h/2) - (myTextPosition.h/2);

        SDL_SetTextureAlphaMod(myText, 230);
    }

    myClicked = false;
    myHovering = false;
    if (clickSFXFile != nullptr)
        myClickSFXID = AudioSystem::LoadClip(clickSFXFile);
    else myClickSFXID = -1;
    if (hoverSFXFile != nullptr)
        myHoverSFXID = AudioSystem::LoadClip(hoverSFXFile);

    SDL_SetTextureAlphaMod(myTexture, 230);
    
}

UI_Button::UI_Button(const char* texturePath, SDL_Renderer* aRenderer, SDL_Rect aPosition, ASPECT_RATIO ratio, const char* text, TTF_Font* aFont, SDL_Color aFontColour, const char* clickSFXFile, const char* hoverSFXFile) : UI_Element(texturePath, aRenderer, aPosition, ratio){
    if (text != nullptr){
        SDL_Surface* tempSurface = TTF_RenderText_Blended(aFont,text,aFontColour);
        myText = SDL_CreateTextureFromSurface(aRenderer, tempSurface);
        SDL_FreeSurface(tempSurface);

        SDL_QueryTexture(myText,NULL,NULL,&myTextPosition.w,&myTextPosition.h);
        myTextPosition.h *= (myPosition.w * 0.8f)/myTextPosition.w;
        myTextPosition.w = myPosition.w * 0.8f;
        myTextPosition.x = myPosition.x + (myPosition.w/2) - (myTextPosition.w/2);
        myTextPosition.y = myPosition.y + (myPosition.h/2) - (myTextPosition.h/2);

        SDL_SetTextureAlphaMod(myText, 230);
    }

    myClicked = false;
    myHovering = false;
    if (clickSFXFile != nullptr)
        myClickSFXID = AudioSystem::LoadClip(clickSFXFile);
    else myClickSFXID = -1;
    if (hoverSFXFile != nullptr)
        myHoverSFXID = AudioSystem::LoadClip(hoverSFXFile);
    else myHoverSFXID = -1;

    SDL_SetTextureAlphaMod(myTexture, 230);
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
    SDL_Point mousePos;
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    if (SDL_PointInRect(&mousePos,&myPosition)){
        if (!myHovering) {
            myHovering = true;
            if (myTexture != nullptr)
                SDL_SetTextureAlphaMod(myTexture, 255);
            if (myText != nullptr)
                SDL_SetTextureAlphaMod(myText, 255);
            if (myHoverSFXID > -1)
                AudioSystem::PlayClip(myHoverSFXID);
        }
        if (SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){
            myClicked = true;
            if (myClickSFXID > -1)
                AudioSystem::PlayClip(myClickSFXID);
        }
    }
    else if (myHovering){
        myHovering = false;
        if (myTexture != nullptr)
            SDL_SetTextureAlphaMod(myTexture, 230);
        if (myText != nullptr)
            SDL_SetTextureAlphaMod(myText, 230);
    }
}

void UI_Button::Render(SDL_Renderer* aRenderer){
    if (myTexture != nullptr)
        SDL_RenderCopy(aRenderer, myTexture, NULL, &myPosition);
    else if (myText != nullptr)
        SDL_RenderCopy(aRenderer, myText, NULL, &myPosition);

    if (myText != nullptr && myTexture != nullptr){
        SDL_RenderCopy(aRenderer, myText, NULL, &myTextPosition);
    }
}