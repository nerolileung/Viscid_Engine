#include "Logo.h"
#include <string>
#include <cmath>
#include <iostream>
#include "include/SDL2/SDL_image.h"

Logo::Logo(const char* filepath, SDL_Renderer* aRenderer, float duration){
    SDL_Surface* tempSurface = IMG_Load(filepath);
    image = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
    SDL_FreeSurface(tempSurface);
    finished = false;
    timerFreeze = duration;
    // ignore these until we have png loading
    timerFadeIn = 1.f;
    timerFadeOut = 1.f;
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
        alpha = std::ceil(255 * ((1.f-timerFadeIn)/1.f));
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
        alpha = std::floor(255 * (timerFadeOut/1.f));
        if (timerFadeOut < 0){
            alpha = 0;
            finished = true;
        }
    }
    SDL_SetTextureAlphaMod(image, alpha);
}

void Logo::Render(SDL_Renderer* aRenderer){
    SDL_Rect position;
    SDL_QueryTexture(image,NULL,NULL,&position.w,&position.h);
    // center image on screen
    position.x = (640-position.w)/2;
    position.y = (480-position.h)/2;
    SDL_RenderCopy(aRenderer,image,NULL,&position);
}