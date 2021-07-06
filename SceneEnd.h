#ifndef SCENEEND_H
#define SCENEEND_H

#include "Scene.h"
#include "framework/UI_Button.h"

class SceneEnd : public Scene {
public:
    SceneEnd();
    ~SceneEnd();
    bool Init(SDL_Renderer* aRenderer) override { return false; };
    bool Init(SDL_Renderer* aRenderer, float duration);
    bool Update(float deltaTime) override;
    void Render(SDL_Renderer* aRenderer) override;
private:
    UI_Element* myBackground;
    UI_Button* myRestartButton;
    UI_Button* myQuitButton;
    UI_Button* myMainMenuButton;
    SDL_Texture* myTime;
    int levelTime;
};

#endif // SCENEEND_H