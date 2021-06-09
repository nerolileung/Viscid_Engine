#include "Logo.h"
#include <string>
#include <cmath>
#include <iostream>

Logo::Logo(const char* filepath, SDL_Renderer* aRenderer, float duration){
    SDL_Surface* tempSurface = SDL_LoadBMP(filepath);
    image = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
    SDL_FreeSurface(tempSurface);
    finished = false;
    timerFreeze = duration;
    timerFadeIn = 5.f;
    timerFadeOut = 5.f;
}

Logo::~Logo(){
    delete image;
    image = nullptr;
}

void Logo::Update(float deltaTime){
    int alpha = 255;
    // fade in logo 
    if (timerFadeIn > 0){
        timerFadeIn -= deltaTime;
        alpha = std::ceil(255 * ((5.f-timerFadeIn)/5.f));
        if (timerFadeIn < 0)
            alpha = 255;
    }
    // render logo at full opacity for given duration
    else if (timerFreeze > 0){
        timerFreeze -= deltaTime;
    }
    // fade out logo
    else {
        timerFadeOut -= deltaTime;
        alpha = std::floor(255 * (timerFadeOut/5.f));
        if (timerFadeOut < 0){
            alpha = 0;
            finished = true;
        }
    }
    SDL_SetTextureAlphaMod(image, alpha);
    std::cout << alpha << std::endl;
}

void Logo::Render(SDL_Renderer* aRenderer){
    SDL_Rect position;
    SDL_QueryTexture(image,NULL,NULL,&position.w,&position.h);
    // center image on screen
    position.x = (640-position.w)/2;
    position.y = (480-position.h)/2;
    SDL_RenderCopy(aRenderer,image,NULL,&position);
}