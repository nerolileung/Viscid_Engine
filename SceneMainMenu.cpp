#include "SceneMainMenu.h"
#include "Game.h"
#include "framework/include/SDL2/SDL_ttf.h"

SceneMainMenu::SceneMainMenu(){
    myFinished = false;
}

SceneMainMenu::~SceneMainMenu(){
    delete myBackground;
    myBackground = nullptr;
    SDL_DestroyTexture(myHeading);
    myHeading = nullptr;
}

bool SceneMainMenu::Init(SDL_Renderer* aRenderer){
    myBackground = new UI_Element("data/mainmenu_bg.png",aRenderer);
    if (myBackground == nullptr) return false;

    SDL_Surface* headingSurface = TTF_RenderText_Blended(gameFonts[0],"HatQuest",gameFontColours[0]);
    myHeading = SDL_CreateTextureFromSurface(aRenderer, headingSurface);
    SDL_FreeSurface(headingSurface);
    if (myHeading == nullptr) return false;

    return true;
}

bool SceneMainMenu::Update(float deltaTime){
    // todo if quit button is pressed return false
    return true;
}

void SceneMainMenu::Render(SDL_Renderer* aRenderer){
    myBackground->Render(aRenderer);
    
    SDL_Rect headingPos;
    headingPos.x = (Game::WindowWidth*0.6f)/2;
    headingPos.y = Game::WindowHeight*0.1f;
    headingPos.w = Game::WindowWidth*0.4f;
    headingPos.h = Game::WindowHeight*0.15f;
    SDL_RenderCopy(aRenderer,myHeading,NULL,&headingPos);
}