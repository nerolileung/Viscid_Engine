#ifndef SCENE_H
#define SCENE_H

#include "framework/include/SDL2/SDL.h"
#include "framework/include/SDL2/SDL_ttf.h"
#include "HatQuest.h"
#include <vector>

class Scene {
public:
    virtual bool Init(SDL_Renderer* aRenderer) = 0;
    virtual bool Update(float deltaTime) = 0;
    virtual void Render(SDL_Renderer* aRenderer) = 0;
    HatQuest::SCENES GetNextScene() { return myNextScene; };
    bool isFinished() { return myFinished; };
    static void SetFonts(std::vector<TTF_Font*> aFonts);
protected:
    bool myFinished;
    HatQuest::SCENES myNextScene;
    static std::vector<TTF_Font*> gameFonts;
    static const std::vector<SDL_Colour> gameFontColours;
};

#endif // SCENE_H