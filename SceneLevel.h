#ifndef SCENELEVEL_H
#define SCENELEVEL_H

#include "Scene.h"
#include "HatQuest.h"

class SceneLevel : public Scene {
public:
    SceneLevel();
    ~SceneLevel();
    bool Init(SDL_Renderer* aRenderer) override;
    bool Update(float deltaTime) override;
    void Render(SDL_Renderer* aRenderer) override;
    void SetPaused(bool paused) { myPaused = paused; };
    HatQuest::SCENES GetNextScene();
private:
    bool myPaused;
    bool myPlayerDead;
    float myBackgroundColour[3];
    int myBackgroundColourChange[3];
    void UpdateBackgroundColour(float deltaTime);
};

#endif // SCENELEVEL_H