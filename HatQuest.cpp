#include "HatQuest.h"
#include "SceneMainMenu.h"

HatQuest::HatQuest(){
    currentSceneType = MAIN_MENU;
    // todo main menu scene cast to general scene
    currentScene = (Scene*)(new SceneMainMenu());
    initialisedScene = false;
}
HatQuest::~HatQuest(){
    delete currentScene;
    currentScene = nullptr;
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

    return currentScene->Update(deltaTime);
}

void HatQuest::Render(SDL_Renderer* aRenderer){
    if (!initialisedScene) currentScene->Init(aRenderer);
    currentScene->Render(aRenderer);
}

void HatQuest::ChangeScene(SCENES newScene){
    // todo switch
    currentSceneType = newScene;
    initialisedScene = false;
}