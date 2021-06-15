#include "Character.h"
#include "framework/Game.h"

Character::Character(){
    myState = PLAYER_STATE::RUNNING;
    myCurrentSpriteIndex = 0;
    mySpriteTimerCurrent = mySpriteTimerMax;
}

Character::~Character(){
}

bool Character::Init(SDL_Renderer* aRenderer, int unitSize){
    gameUnit = unitSize;
    // running sprite dimensions
    myPosition.w = gameUnit;
    myPosition.h = gameUnit * 5 / 3;
    myPosition.x = (gameUnit * 2) - (myPosition.w / 2);
    myPosition.y = (gameUnit * 7) - (myPosition.h / 2);
    
    if (!InitSprites(aRenderer)) return false;

    return true;
}

bool Character::InitSprites(SDL_Renderer* aRenderer){
    mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element("data/player_1.png", aRenderer, myPosition, UI_Element::ASPECT_RATIO::WIDTH)));
    if (!mySprites[0]) return false;
    mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element("data/player_2.png", aRenderer, myPosition, UI_Element::ASPECT_RATIO::WIDTH)));
    if (!mySprites[1]) return false;

    return true;
}

void Character::Update(float deltaTime){
    mySpriteTimerCurrent -= deltaTime;
    if (mySpriteTimerCurrent < 0){
        if (myCurrentSpriteIndex % 2 == 0) myCurrentSpriteIndex++;
        else myCurrentSpriteIndex--;
        mySpriteTimerCurrent += mySpriteTimerMax;
    }

    // handle input

    // handle vertical movement

    // fell
    if (myPosition.y < Game::WindowHeight - (myPosition.h/2))
        ChangeState(PLAYER_STATE::DEAD);

    // hit a wall
    bool hasCollided = false;
    // todo get tiles adjacent to player and check collisions
    if (hasCollided) ChangeState(PLAYER_STATE::DEAD);
}

void Character::Render(SDL_Renderer* aRenderer){
    mySprites[myCurrentSpriteIndex]->Render(aRenderer);
}

void Character::ChangeState(PLAYER_STATE aState){
    switch (aState){
        case PLAYER_STATE::RUNNING:
            myPosition.w = gameUnit;
            myPosition.h = gameUnit * 5 / 3;
            myPosition.x = (gameUnit * 2) - (myPosition.w / 2);
            myPosition.y = (gameUnit * 7) - (myPosition.h / 2);
            mySprites[0]->SetPosition({myPosition.x,myPosition.y});
            mySprites[1]->SetPosition({myPosition.x,myPosition.y});
        break;
        case PLAYER_STATE::JUMPING:
        break;
        case PLAYER_STATE::SLIDING:
            myPosition.h = gameUnit;
        break;
        case PLAYER_STATE::DEAD:
        break;
    }
    myState = aState;
    myCurrentSpriteIndex = myState;
}