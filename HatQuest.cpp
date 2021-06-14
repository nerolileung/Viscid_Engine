#include "HatQuest.h"
#include "SceneMainMenu.h"
#include <iostream>

HatQuest::HatQuest(){
    currentSceneType = MAIN_MENU;
    currentScene = (Scene*)(new SceneMainMenu());
    initialisedScene = false;
    fonts.push_back(TTF_OpenFont("data/caprice-medium.ttf", 200));
    fonts.push_back(TTF_OpenFont("data/caprice-medium.ttf", 50));
    Scene::SetFonts(fonts);
}
HatQuest::~HatQuest(){
    delete currentScene;
    currentScene = nullptr;
    for (int i = 0; i < 2; i++){
        TTF_CloseFont(fonts[i]);
        fonts[i] = nullptr;
    }
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

    if (initialisedScene)
        return currentScene->Update(deltaTime);
    else return true;
}

void HatQuest::Render(SDL_Renderer* aRenderer){
    if (!initialisedScene){
        initialisedScene = currentScene->Init(aRenderer);
        if (!initialisedScene) {
            std::cout << "Error initialising scene!";
            return;
        }
    }
    currentScene->Render(aRenderer);
}

void HatQuest::ChangeScene(SCENES newScene){
    // todo switch
    currentSceneType = newScene;
    initialisedScene = false;
}