#include "SceneLevel.h"
#include <cmath>

SceneLevel::SceneLevel(){
    myFinished = false;
    myPaused = false;

    myBackgroundColour[0] = 28.f - std::rand()%4;
    myBackgroundColour[1] = 150.f + std::rand()%20;
    myBackgroundColour[2] = 201.f - std::rand()%50;
    myBackgroundColourChange[0] = 5;
    myBackgroundColourChange[1] = 10;
    myBackgroundColourChange[2] = -10;

    myTileSize = Game::WindowHeight/8; // 8 tiles up, 9-32 tiles across (avg. 14)
    myPlayer = new Character();
    // todo tile pool
}

SceneLevel::~SceneLevel(){
    delete myPlayer;
    myPlayer = nullptr;
}

bool SceneLevel::Init(SDL_Renderer* aRenderer){
    myPlayer->Init(aRenderer,myTileSize);
    if (myPlayer == nullptr) return false;

    return true;
}

bool SceneLevel::Update(float deltaTime){
    if (myPaused){
        // update pause menu ui and nothing else
        // if return to main menu button is pressed, set finished
    }
    else {
        UpdateBackgroundColour(deltaTime);
        // move world
        myPlayer->Update(deltaTime);
        myFinished = myPlayer->isDead();
    }
    return true;
}

void SceneLevel::UpdateBackgroundColour(float deltaTime){
    myBackgroundColour[0] += myBackgroundColourChange[0] * deltaTime;
    if (myBackgroundColour[0] < 0) myBackgroundColour[0] = 0;
    if (myBackgroundColour[0] > 150) myBackgroundColour[0] = 150;

    myBackgroundColour[1] += myBackgroundColourChange[1] * deltaTime;
    if (myBackgroundColour[1] < 0) myBackgroundColour[1] = 0;

    myBackgroundColour[2] += myBackgroundColourChange[2] * deltaTime;
    if (myBackgroundColour[2] < 0) myBackgroundColour[2] = 0;
    
    // green to blue
    if (myBackgroundColour[1] >= 200){
        myBackgroundColourChange[0] = -5;
        myBackgroundColourChange[1] = -20;
        myBackgroundColourChange[2] = 20;
    }
    // blue to purple
    else if (myBackgroundColourChange[2] == 20 && myBackgroundColour[2] > 160){
        myBackgroundColourChange[0] = 30;
        myBackgroundColourChange[1] = -30;
        myBackgroundColourChange[2] = 15;
    }
    // purple to blue
    else if (myBackgroundColour[2] >= 200){
        myBackgroundColourChange[0] = -20;
        myBackgroundColourChange[1] = 10;
        myBackgroundColourChange[2] = -10;
    }
    // blue to green
    else if (myBackgroundColourChange[0] == -20 && myBackgroundColour[0] < 0){
        myBackgroundColourChange[0] = 5;
        myBackgroundColourChange[1] = 10;
        myBackgroundColourChange[2] = -10;
    }
}

void SceneLevel::Render(SDL_Renderer* aRenderer){
    // background changes colour
    SDL_SetRenderDrawColor(aRenderer, myBackgroundColour[0], myBackgroundColour[1], myBackgroundColour[2], SDL_ALPHA_OPAQUE);
    SDL_RenderClear(aRenderer);

    // render regular screen
    myPlayer->Render(aRenderer);

    if (myPaused){
        // render pause menu ui
    }
}

HatQuest::SCENES SceneLevel::GetNextScene(){
    if (myPaused) return HatQuest::SCENES::MAIN_MENU;
    if (myPlayer->isDead()) return HatQuest::SCENES::LOSE;
    else return HatQuest::SCENES::WIN;
}