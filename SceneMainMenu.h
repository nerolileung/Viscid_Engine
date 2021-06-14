#ifndef SCENEMAINMENU_H
#define SCENEMAINMENU_H

#include "Scene.h"
#include "framework/UI_Element.h"
#include "framework/UI_Button.h"

class SceneMainMenu : public Scene {
public:
    SceneMainMenu();
    ~SceneMainMenu();
    bool Init(SDL_Renderer* aRenderer) override;
    bool Update(float deltaTime) override;
    void Render(SDL_Renderer* aRenderer) override;
private:
    UI_Element* myBackground;
    SDL_Texture* myHeading;
    UI_Button* myQuitButton;
};

#endif // SCENEMAINMENU_H