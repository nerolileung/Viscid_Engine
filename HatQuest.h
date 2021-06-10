#ifndef HATQUEST_H
#define HATQUEST_H

#include "framework/Game.h"
#include "Scene.h"

class HatQuest : public Game
{
public:
    HatQuest();
    ~HatQuest();
    bool Update(float deltaTime);
    void Render(SDL_Renderer* aRenderer);
private:
    enum SCENES {
        MAIN_MENU = 0,
        PLAYING,
        WIN,
        LOSE
    };
    SCENES currentSceneType;
    Scene* currentScene;
    bool paused;
    void ChangeScene(SCENES newScene);
    SDL_Texture* myLogo;
};

#endif // HATQUEST_H