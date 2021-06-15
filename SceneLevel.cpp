#include "SceneLevel.h"

SceneLevel::SceneLevel(){
    myFinished = false;
    myPaused = false;
}

SceneLevel::~SceneLevel(){
}

bool SceneLevel::Init(SDL_Renderer* aRenderer){
    myPlayerDead = false;
    return true;
}

bool SceneLevel::Update(float deltaTime){
    if (myPaused){
        // update pause menu ui and nothing else
        // if return to main menu button is pressed, set finished
    }
    else {
        // game logic
    }
    return true;
}

void SceneLevel::Render(SDL_Renderer* aRenderer){
    // render regular screen
    if (myPaused){
        // render pause menu ui
    }
}

HatQuest::SCENES SceneLevel::GetNextScene(){
    if (myPaused) return HatQuest::SCENES::MAIN_MENU;
    if (myPlayerDead) return HatQuest::SCENES::LOSE;
    else return HatQuest::SCENES::WIN;
}