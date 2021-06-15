#ifndef CHARACTER_H
#define CHARACTER_H

#include "framework/include/SDL2/SDL.h"
#include "framework/UI_Element.h"
#include <vector>
#include <memory>

class Character {
public:
    Character();
    ~Character();
    bool Init(SDL_Renderer* aRenderer, int unitSize);
    bool Update(float deltaTime);
    void Render(SDL_Renderer* aRenderer);
    bool isDead() { return myDead; };
private:
    bool myDead;
    std::vector<std::unique_ptr<UI_Element>> mySprites;
    int myCurrentSpriteIndex;
    float mySpriteTimerCurrent;
    const float mySpriteTimerMax = 1.f;
    int gameUnit;
    SDL_Rect myPosition;
};

#endif // CHARACTER_H