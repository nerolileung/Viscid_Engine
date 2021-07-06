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

    mainMenuBGM = Mix_LoadMUS("data/mainmenu_bgm.wav");
    levelBGM = Mix_LoadMUS("data/level_bgm.wav");
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

    Mix_FadeOutMusic(10);
    Mix_FreeMusic(mainMenuBGM);
    mainMenuBGM = nullptr;
    Mix_FreeMusic(levelBGM);
    levelBGM = nullptr;
}

bool HatQuest::Update(float deltaTime){
    // start playing bgm on first update after logos get displayed
    if (myCurrentSceneType == SCENES::MAIN_MENU && 
    (Mix_PlayingMusic() == 0 && Mix_FadingMusic() != MIX_FADING_IN)){
        Mix_FadeInMusic(mainMenuBGM, -1, 100);
    }

    // move to new scene
    if (myCurrentScene->isFinished()){
        switch (myCurrentSceneType){
            case MAIN_MENU:
            case END:
                ChangeScene(myCurrentScene->GetNextScene());
            break;
            case PLAYING:
                ChangeScene(((SceneLevel*)myCurrentScene)->GetNextScene(levelTime));
                if (levelTime > 10) myTutorialDone = true;
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
    Mix_HaltMusic();
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
            Mix_FadeInMusic(mainMenuBGM, -1, 100);
            myCurrentScene = (Scene*)(new SceneMainMenu());
        break;
        case PLAYING:
            Mix_FadeInMusic(levelBGM, -1, 100);
            myCurrentScene = (Scene*)(new SceneLevel());
        break;
        case END:
            Mix_FadeInMusic(mainMenuBGM, -1, 100);
            myCurrentScene = (Scene*)(new SceneEnd());
        break;
    }
    myCurrentSceneType = newSceneType;
    initialisedScene = false;
}