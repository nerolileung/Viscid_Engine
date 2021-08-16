#ifndef HATQUEST_H
#define HATQUEST_H

#include "framework/Game.h"
#include "framework/include/SDL2/SDL_ttf.h"
#include "framework/include/SDL2/SDL_mixer.h"
#include <vector>

class Scene;

class HatQuest final : public Game
{
public:
    HatQuest();
    ~HatQuest();
    bool Update(float deltaTime);
    void Render(SDL_Renderer* aRenderer);
    enum SCENES {
        MAIN_MENU = 0,
        SETTINGS_MENU,
        PLAYING,
        END
    };
private:
    SCENES myCurrentSceneType;
    Scene* myCurrentScene;
    void ChangeScene(SCENES newSceneType);
    bool initialisedScene;

    std::vector<TTF_Font*> fonts;
    float levelTime;
    bool myTutorialDone;

    Mix_Music* menuBGM;
};

#endif // HATQUEST_H