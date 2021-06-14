#include "SceneMainMenu.h"
#include "Game.h"

SceneMainMenu::SceneMainMenu(){
    myFinished = false;
}

SceneMainMenu::~SceneMainMenu(){
    delete myBackground;
    myBackground = nullptr;
}

bool SceneMainMenu::Init(SDL_Renderer* aRenderer){
    myBackground = new UI_Element("data/mainmenu_bg.png",aRenderer);
    if (myBackground == nullptr) return false;
    return true;
}

bool SceneMainMenu::Update(float deltaTime){
    return true;
}

void SceneMainMenu::Render(SDL_Renderer* aRenderer){
    myBackground->Render(aRenderer);
}