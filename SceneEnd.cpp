#include "SceneEnd.h"
#include <cmath>
#include <string>

SceneEnd::SceneEnd(){
    myFinished = false;
}

SceneEnd::~SceneEnd(){
    delete myBackground;
    myBackground = nullptr;

    delete myRestartButton;
    myRestartButton = nullptr;

    delete myMainMenuButton;
    myMainMenuButton = nullptr;
    
    delete myQuitButton;
    myQuitButton = nullptr;

    SDL_DestroyTexture(myTime);
    myTime = nullptr;
}

bool SceneEnd::Init(SDL_Renderer* aRenderer, float duration){
    levelTime = std::round(duration); // milliseconds to seconds
    int minutes = (levelTime - (levelTime % 60)) / 60;
    int remainingSeconds = levelTime % 60;
    std::string timeDisplay = "Final Time: " + std::to_string(minutes) + ":" + std::to_string(remainingSeconds);
    if (remainingSeconds == 0) timeDisplay+="0";

    SDL_Surface* timeSurface = TTF_RenderText_Blended(gameFonts[0],timeDisplay.c_str(),gameFontColours[0]);
    myTime = SDL_CreateTextureFromSurface(aRenderer, timeSurface);
    SDL_FreeSurface(timeSurface);
    if (myTime == nullptr) return false;

    myBackground = new UI_Element("data/end_bg.png",aRenderer);
    if (myBackground == nullptr) return false;

    SDL_Rect buttonPosition = {(int)(Game::WindowWidth*0.3f), (int)(Game::WindowHeight*0.2f), (int)(Game::WindowHeight*0.3f), (int)(Game::WindowHeight*0.3f)};
    myRestartButton = new UI_Button("data/mainmenu_button.png","Play Again",gameFonts[1],gameFontColours[0],aRenderer,buttonPosition,UI_Element::ASPECT_RATIO::NONE);
    if (myRestartButton == nullptr) return false;

    buttonPosition.x = Game::WindowWidth*0.7f;
    myMainMenuButton = new UI_Button("data/mainmenu_button.png","Main Menu",gameFonts[1],gameFontColours[0],aRenderer,buttonPosition,UI_Element::ASPECT_RATIO::NONE);
    if (myMainMenuButton == nullptr) return false;

    buttonPosition.x = Game::WindowWidth*0.5f;
    buttonPosition.y = Game::WindowHeight*0.8f;
    myQuitButton = new UI_Button("data/mainmenu_button.png","  Quit  ",gameFonts[1],gameFontColours[0],aRenderer,buttonPosition,UI_Element::ASPECT_RATIO::NONE);
    if (myQuitButton == nullptr) return false;

    return true;
}

bool SceneEnd::Update(float deltaTime){
    myRestartButton->Update(deltaTime);
    myQuitButton->Update(deltaTime);
    myMainMenuButton->Update(deltaTime);

    if (myRestartButton->isClicked()){
        myNextScene = HatQuest::SCENES::PLAYING;
        myFinished = true;
    }
    else if (myQuitButton->isClicked())
        return false;
    else if (myMainMenuButton->isClicked()){
        myNextScene = HatQuest::SCENES::MAIN_MENU;
        myFinished = true;
    }
    
    return true;
}

void SceneEnd::Render(SDL_Renderer* aRenderer){
    myBackground->Render(aRenderer);
    myRestartButton->Render(aRenderer);
    myQuitButton->Render(aRenderer);
    myMainMenuButton->Render(aRenderer);

    SDL_Rect timePos;
    SDL_QueryTexture(myTime,NULL,NULL,&timePos.w,&timePos.h);
    timePos.w *= Game::WindowHeight*0.1f / timePos.h;
    timePos.h = Game::WindowHeight*0.1f;
    timePos.y = Game::WindowHeight*0.5f - (timePos.h / 2);
    timePos.x = Game::WindowWidth*0.5f - (timePos.w / 2);
    SDL_RenderCopy(aRenderer,myTime,NULL,&timePos);
}