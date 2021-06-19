#include "SceneEnd.h"

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
}

bool SceneEnd::Init(SDL_Renderer* aRenderer){
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
}