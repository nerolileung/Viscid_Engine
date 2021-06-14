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
    myPosition.x = xPos;
    myPosition.y = yPos;
    SDL_QueryTexture(myTexture,NULL,NULL,&myPosition.w,&myPosition.h);
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