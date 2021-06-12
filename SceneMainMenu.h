#ifndef SCENEMAINMENU_H
#define SCENEMAINMENU_H

#include "Scene.h"

class SceneMainMenu : public Scene {
public:
    SceneMainMenu();
    ~SceneMainMenu();
    bool Update(float deltaTime);
    void Render(SDL_Renderer* aRenderer);
};

#endif // SCENEMAINMENU_H