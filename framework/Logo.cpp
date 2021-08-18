#include "Logo.h"
#include <string>
#include <cmath>
#include "include/SDL2/SDL_image.h"
#include "framework/AudioSystem.h"

Logo::Logo(const char* imagePath, SDL_Renderer* aRenderer, const char* sfxPath, float duration, float sfxOffset){
    // load image
    SDL_Surface* tempSurface = IMG_Load(imagePath);
    image = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
    SDL_FreeSurface(tempSurface);

    // load audio (if present)
    if (sfxPath != nullptr)
        sfx = AudioSystem::LoadClip(sfxPath);
    else sfx = -1;

    // variables
    finished = false;
    timerFreeze = duration;
    timerFadeIn = 1.f;
    timerFadeOut = 1.f;
    sfxTimestamp = sfxOffset;
    sfxPlayed = false;
}

Logo::~Logo(){
    SDL_DestroyTexture(image);
    image = nullptr;
    AudioSystem::UnloadClip(sfx);
}

void Logo::Update(float deltaTime){
    // visuals
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
    
    // audio
    if (sfx > -1){
        sfxTimestamp -= deltaTime;
        if (sfxTimestamp < 0 && !sfxPlayed){
            AudioSystem::PlayClip(sfx);
            sfxPlayed = true;
        }
    }
}

void Logo::Render(SDL_Renderer* aRenderer, SDL_Window* aWindow){
    // center image on screen
    SDL_Rect position;
    SDL_QueryTexture(image,NULL,NULL,&position.w,&position.h);
    SDL_GetWindowSize(aWindow, &position.x, &position.y);
    position.x = (position.x-position.w)/2;
    position.y = (position.y-position.h)/2;
    SDL_RenderCopy(aRenderer,image,NULL,&position);
}