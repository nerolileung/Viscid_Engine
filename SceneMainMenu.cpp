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
    delete myQuitButton;
    myQuitButton = nullptr;
}

bool SceneMainMenu::Init(SDL_Renderer* aRenderer){
    myBackground = new UI_Element("data/mainmenu_bg.png",aRenderer);
    if (myBackground == nullptr) return false;

    SDL_Surface* headingSurface = TTF_RenderText_Blended(gameFonts[0],"HatQuest",gameFontColours[0]);
    myHeading = SDL_CreateTextureFromSurface(aRenderer, headingSurface);
    SDL_FreeSurface(headingSurface);
    if (myHeading == nullptr) return false;

    myQuitButton = new UI_Button("Quit",gameFonts[1],gameFontColours[0],aRenderer,Game::WindowWidth/2, Game::WindowHeight*0.8f);
    if (myQuitButton == nullptr) return false;

    return true;
}

bool SceneMainMenu::Update(float deltaTime){
    myQuitButton->Update(deltaTime);
    return !myQuitButton->isClicked();
}

void SceneMainMenu::Render(SDL_Renderer* aRenderer){
    myBackground->Render(aRenderer);
    
    SDL_Rect headingPos;
    headingPos.x = (Game::WindowWidth*0.6f)/2;
    headingPos.y = Game::WindowHeight*0.1f;
    headingPos.w = Game::WindowWidth*0.4f;
    headingPos.h = Game::WindowHeight*0.15f;
    SDL_RenderCopy(aRenderer,myHeading,NULL,&headingPos);

    myQuitButton->Render(aRenderer);
}