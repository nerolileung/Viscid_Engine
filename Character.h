#ifndef CHARACTER_H
#define CHARACTER_H

#include "framework/include/SDL2/SDL.h"
#include "framework/UI_Element.h"
#include <vector>
#include <memory>
#include "TilePooler.h"

class Character {
public:
    Character();
    ~Character();
    bool Init(SDL_Renderer* aRenderer, int unitSize, TilePooler* aTilePooler);
    void Update(float deltaTime, float speed);
    void Render(SDL_Renderer* aRenderer);
    bool isDead();
private:
    enum PLAYER_STATE {
        RUNNING = 0,
        JUMPING = 2,
        SLIDING = 4,
        DEAD = 6
    };
    PLAYER_STATE myState;
    void ChangeState(PLAYER_STATE aState);

    bool InitSprites(SDL_Renderer* aRenderer);
    std::vector<std::unique_ptr<UI_Element>> mySprites;
    int myCurrentSpriteIndex;
    float mySpriteTimerCurrent;
    float mySpriteTimerMax;

    float myJumpTimer;

    int gameUnit;
    SDL_Rect myPosition;
    TilePooler* tilePooler;

    void UpdatePosition(float deltaTime, float speed);
};

#endif // CHARACTER_H