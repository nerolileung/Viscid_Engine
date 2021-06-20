#ifndef HATQUEST_H
#define HATQUEST_H

#include "framework/Game.h"
#include "framework/include/SDL2/SDL_ttf.h"
#include "Scene.h"
#include <vector>

class HatQuest final : public Game
{
public:
    HatQuest();
    ~HatQuest();
    bool Update(float deltaTime);
    void Render(SDL_Renderer* aRenderer);
    enum SCENES {
        MAIN_MENU = 0,
        PLAYING,
        END
    };
private:
    SCENES myCurrentSceneType;
    Scene* myCurrentScene;
    bool initialisedScene;
    float levelTime;
    void ChangeScene(SCENES newSceneType);
    std::vector<TTF_Font*> fonts;
    bool myTutorialDone;
};

#endif // HATQUEST_H