#include "UI_Element.h"

UI_Element::UI_Element(){
    myTexture = nullptr;
    SetSize({0,0},ASPECT_RATIO::NONE);
}

UI_Element::UI_Element(const char* texturePath, SDL_Renderer* aRenderer){
    SDL_Surface* tempSurface = IMG_Load(texturePath);
    myTexture = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
    SDL_FreeSurface(tempSurface);
    SetSize({0,0},ASPECT_RATIO::NONE);
}

UI_Element::UI_Element(const char* texturePath, SDL_Renderer* aRenderer, float xPos, float yPos){
    SDL_Surface* tempSurface = IMG_Load(texturePath);
    myTexture = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
    SDL_FreeSurface(tempSurface);
    SDL_QueryTexture(myTexture,NULL,NULL,&myPosition.w,&myPosition.h);
    SetPosition({(int)xPos,(int)yPos});
}

UI_Element::UI_Element(const char* texturePath, SDL_Renderer* aRenderer, SDL_Rect aPosition, ASPECT_RATIO ratio){
    SDL_Surface* tempSurface = IMG_Load(texturePath);
    myTexture = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
    SDL_FreeSurface(tempSurface);
    // scale element based on given ratio and dimensions
    SDL_QueryTexture(myTexture,NULL,NULL,&myPosition.w,&myPosition.h);
    SetSize({aPosition.w, aPosition.h},ratio);
    SetPosition({aPosition.x,aPosition.y});
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
    // offset so given position is midpoint of element
    myPosition.x = aPosition.x - (myPosition.w / 2);
    myPosition.y = aPosition.y - (myPosition.h / 2);
}

void UI_Element::SetSize(SDL_Point aSize, ASPECT_RATIO ratio){
    switch (ratio){
        case ASPECT_RATIO::NONE:
            myPosition.w = aSize.x;
            myPosition.h = aSize.y;
        break;
        case ASPECT_RATIO::HEIGHT:
            myPosition.w = myPosition.w * aSize.y / myPosition.h;
            myPosition.h = aSize.y;
        break;
        case ASPECT_RATIO::WIDTH:
            myPosition.h = myPosition.h * aSize.x / myPosition.w;
            myPosition.w = aSize.x;
        break;
    }
}