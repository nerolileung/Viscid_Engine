#include "HatQuest.h"
#include "SceneMainMenu.h"
#include "SceneLevel.h"
#include "SceneEnd.h"
#include <iostream>

HatQuest::HatQuest(){
    myCurrentSceneType = MAIN_MENU;
    myCurrentScene = (Scene*)(new SceneMainMenu());
    initialisedScene = false;
    levelTime = 0.f;
    myTutorialDone = false;
    fonts.push_back(TTF_OpenFont("data/caprice-medium.ttf", 200));
    fonts.push_back(TTF_OpenFont("data/caprice-medium.ttf", 50));
    Scene::SetFonts(fonts);
}
HatQuest::~HatQuest(){
    switch (myCurrentSceneType){
        case MAIN_MENU:
            delete (SceneMainMenu*)myCurrentScene;
        break;
        case PLAYING:
            delete (SceneLevel*)myCurrentScene;
        break;
        case END:
            delete (SceneEnd*)myCurrentScene;
        break;
    }
    myCurrentScene = nullptr;
    for (int i = 0; i < 2; i++){
        TTF_CloseFont(fonts[i]);
        fonts[i] = nullptr;
    }
}

bool HatQuest::Update(float deltaTime){
    // move to new scene
    if (myCurrentScene->isFinished()){
        switch (myCurrentSceneType){
            case MAIN_MENU:
            ChangeScene(PLAYING);
            break;
            case PLAYING:
            ChangeScene(((SceneLevel*)myCurrentScene)->GetNextScene(levelTime));
            break;
            case END:
            ChangeScene(((SceneEnd*)myCurrentScene)->GetNextScene());
            break;
        }
    }

    if (initialisedScene)
        return myCurrentScene->Update(deltaTime);
    else return true;
}

void HatQuest::Render(SDL_Renderer* aRenderer){
    if (!initialisedScene){
        switch (myCurrentSceneType){
            case SCENES::END:
                initialisedScene = ((SceneEnd*)myCurrentScene)->Init(aRenderer,levelTime);
            break;
            case SCENES::PLAYING:
                initialisedScene = ((SceneLevel*)myCurrentScene)->Init(aRenderer,!myTutorialDone);
                if (!myTutorialDone) myTutorialDone = initialisedScene;
            break;
            default:
                initialisedScene = myCurrentScene->Init(aRenderer);
            break;
        }
        if (!initialisedScene) {
            std::cout << "Error initialising scene!";
            return;
        }
    }
    else myCurrentScene->Render(aRenderer);
}

void HatQuest::ChangeScene(SCENES newSceneType){
    // allow changing to current scene for reloading
    if (myCurrentScene != nullptr){
        switch (myCurrentSceneType){
            case MAIN_MENU:
                delete (SceneMainMenu*)myCurrentScene;
            break;
            case PLAYING:
                delete (SceneLevel*)myCurrentScene;
            break;
            case END:
                delete (SceneEnd*)myCurrentScene;
            break;
        }
    }
    switch (newSceneType){
        case MAIN_MENU:
            myCurrentScene = (Scene*)(new SceneMainMenu());
        break;
        case PLAYING:
            myCurrentScene = (Scene*)(new SceneLevel());
        break;
        case END:
            myCurrentScene = (Scene*)(new SceneEnd());
        break;
    }
    myCurrentSceneType = newSceneType;
    initialisedScene = false;
}