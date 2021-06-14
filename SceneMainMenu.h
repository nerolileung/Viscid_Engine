#ifndef SCENEMAINMENU_H
#define SCENEMAINMENU_H

#include "Scene.h"
#include "framework/UI_Element.h"

class SceneMainMenu : public Scene {
public:
    SceneMainMenu();
    ~SceneMainMenu();
    bool Init(SDL_Renderer* aRenderer);
    bool Update(float deltaTime);
    void Render(SDL_Renderer* aRenderer);
private:
    UI_Element* myBackground;
};

#endif // SCENEMAINMENU_H