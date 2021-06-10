#include "HatQuest.h"

HatQuest::HatQuest(){
    currentSceneType = MAIN_MENU;
    // todo main menu scene cast to general scene
    currentScene = nullptr;
    myLogo = nullptr;
}
HatQuest::~HatQuest(){

}

bool HatQuest::Update(float deltaTime){
    //pause on escape
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_ESCAPE])
      paused = !paused;
    
    // move to new scene
    if (currentScene->isFinished()){
        switch (currentSceneType){
            case MAIN_MENU:
            ChangeScene(PLAYING);
            break;
            case PLAYING:
            // todo get data from scene
            ChangeScene(WIN);
            break;
            case WIN:
            case LOSE:
            // todo get data from scene
            ChangeScene(MAIN_MENU);
            break;
            default:
                return false;
            break;
        }
    }

    //currentScene->Update(deltaTime);

    return true;
}

void HatQuest::Render(SDL_Renderer* aRenderer){
    // load image if it hasn't loaded yet
    if (!myLogo){
        SDL_Surface* tempSurface = SDL_LoadBMP("data/icon.bmp");
        myLogo = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
        SDL_FreeSurface(tempSurface);
    }

    // render image in the middle of the screen
    SDL_Rect position;
    SDL_QueryTexture(myLogo,NULL,NULL,&position.w,&position.h);
    position.x = (WindowWidth-position.w)/2;
    position.y = (WindowHeight-position.h)/2;

    // pass image to renderer
    SDL_RenderCopy(aRenderer,myLogo,NULL,&position);
}

void HatQuest::ChangeScene(SCENES newScene){
    // todo switch
    currentSceneType = newScene;
}