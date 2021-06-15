#include "SceneMainMenu.h"
#include "Game.h"
#include "framework/include/SDL2/SDL_ttf.h"
#include <cmath>

SceneMainMenu::SceneMainMenu(){
    myFinished = false;
}

SceneMainMenu::~SceneMainMenu(){
    delete myBackground;
    myBackground = nullptr;
    SDL_DestroyTexture(myHeading);
    myHeading = nullptr;
    delete myQuitButton;
    myQuitButton = nullptr;
    delete myPlayButton;
    myPlayButton = nullptr;
}

bool SceneMainMenu::Init(SDL_Renderer* aRenderer){
    myBackground = new UI_Element("data/mainmenu_bg.png",aRenderer);
    if (myBackground == nullptr) return false;

    SDL_Surface* headingSurface = TTF_RenderText_Blended(gameFonts[0],"HatQuest",gameFontColours[0]);
    myHeading = SDL_CreateTextureFromSurface(aRenderer, headingSurface);
    SDL_FreeSurface(headingSurface);
    if (myHeading == nullptr) return false;

    myPlayButton = new UI_Button("data/mainmenu_button.png","Play",gameFonts[1],gameFontColours[0],aRenderer,Game::WindowWidth*0.3f, Game::WindowHeight*0.5f);
    if (myPlayButton == nullptr) return false;

    myQuitButton = new UI_Button("data/mainmenu_button.png","Quit",gameFonts[1],gameFontColours[0],aRenderer, Game::WindowWidth*0.7f,Game::WindowHeight*0.7f);
    if (myQuitButton == nullptr) return false;

    return true;
}

bool SceneMainMenu::Update(float deltaTime){
    myPlayButton->Update(deltaTime);
    myQuitButton->Update(deltaTime);
    if (myPlayButton->isClicked()) myFinished = true;
    return !myQuitButton->isClicked();
}

void SceneMainMenu::Render(SDL_Renderer* aRenderer){
    myBackground->Render(aRenderer);
    
    SDL_Rect headingPos;
    headingPos.x = (Game::WindowWidth*0.7f)/2;
    headingPos.y = Game::WindowHeight*0.1f;
    SDL_QueryTexture(myHeading,NULL,NULL,&headingPos.w,&headingPos.h);
    headingPos.h *= Game::WindowWidth*0.3f/headingPos.w;
    headingPos.w = Game::WindowWidth*0.3f;
    SDL_RenderCopy(aRenderer,myHeading,NULL,&headingPos);

    myPlayButton->Render(aRenderer);
    myQuitButton->Render(aRenderer);
}