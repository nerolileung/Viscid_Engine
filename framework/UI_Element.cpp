#include "UI_Element.h"

UI_Element::UI_Element(){
    myTexture = nullptr;
    myPosition.w = 0;
    myPosition.h = 0;
}

UI_Element::UI_Element(const char* texturePath, SDL_Renderer* aRenderer){
    SDL_Surface* tempSurface = IMG_Load(texturePath);
    myTexture = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
    SDL_FreeSurface(tempSurface);
    myPosition.w = 0;
    myPosition.h = 0;
}

UI_Element::UI_Element(const char* texturePath, SDL_Renderer* aRenderer, float xPos, float yPos){
    SDL_Surface* tempSurface = IMG_Load(texturePath);
    myTexture = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
    SDL_FreeSurface(tempSurface);
    SDL_QueryTexture(myTexture,NULL,NULL,&myPosition.w,&myPosition.h);
    // offset so given position is midpoint of element
    myPosition.x = xPos - (myPosition.w / 2);
    myPosition.y = yPos - (myPosition.h / 2);
}

UI_Element::UI_Element(const char* texturePath, SDL_Renderer* aRenderer, SDL_Rect aPosition, ASPECT_RATIO ratio){
    SDL_Surface* tempSurface = IMG_Load(texturePath);
    myTexture = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
    SDL_FreeSurface(tempSurface);
    // scale element based on given ratio and dimensions
    SDL_QueryTexture(myTexture,NULL,NULL,&myPosition.w,&myPosition.h);
    switch (ratio){
        case ASPECT_RATIO::NONE:
            myPosition.w = aPosition.w;
            myPosition.h = aPosition.h;
        break;
        case ASPECT_RATIO::HEIGHT:
            myPosition.w = myPosition.w * aPosition.h / myPosition.h;
            myPosition.h = aPosition.h;
        break;
        case ASPECT_RATIO::WIDTH:
            myPosition.h = myPosition.h * aPosition.w / myPosition.w;
            myPosition.w = aPosition.w;
        break;
    }
    // offset so given position is midpoint of element
    myPosition.x = aPosition.x - (myPosition.w / 2);
    myPosition.y = aPosition.y - (myPosition.h / 2);
}

UI_Element::~UI_Element(){
    if (myTexture != nullptr){
        SDL_DestroyTexture(myTexture);
        myTexture = nullptr;
    }
}

void UI_Element::Update(float deltaTime){
}

void UI_Element::Render(SDL_Renderer* aRenderer){
    if (myTexture != nullptr){
        if (SDL_RectEmpty(&myPosition))
            SDL_RenderCopy(aRenderer,myTexture,NULL,NULL);
        else SDL_RenderCopy(aRenderer,myTexture,NULL,&myPosition);
    }
}

void UI_Element::SetPosition(SDL_Point aPosition){
    myPosition.x = aPosition.x - (myPosition.w / 2);
    myPosition.y = aPosition.y - (myPosition.h / 2);
}