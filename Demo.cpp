#include "Demo.h"

Demo::Demo(){
    myLogo = nullptr;
}

bool Demo::Update(float deltaTime){
    //check if escape has been pressed
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_ESCAPE])
      return false;
    
    // game hasn't quit, do game logic
    return true;
}

void Demo::Render(SDL_Renderer* aRenderer){
    // load image if it hasn't loaded yet
    if (!myLogo){
        SDL_Surface* tempSurface = SDL_LoadBMP("data/icon.bmp");
        myLogo = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
        SDL_FreeSurface(tempSurface);
    }

    // render image in the middle of the screen
    SDL_Rect position;
    SDL_QueryTexture(myLogo,NULL,NULL,&position.w,&position.h);
    position.x = (640-position.w)/2;
    position.y = (480-position.h)/2;

    // pass image to renderer
    SDL_RenderCopy(aRenderer,myLogo,NULL,&position);
}