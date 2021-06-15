#include "Character.h"
#include "framework/Game.h"

Character::Character(){
    myDead = false;
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
    
    mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element("data/player_1.png", aRenderer, myPosition, UI_Element::ASPECT_RATIO::WIDTH)));
    if (!mySprites[0]) return false;
    mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element("data/player_2.png", aRenderer, myPosition, UI_Element::ASPECT_RATIO::WIDTH)));
    if (!mySprites[1]) return false;

    return true;
}

bool Character::Update(float deltaTime){
    mySpriteTimerCurrent -= deltaTime;
    if (mySpriteTimerCurrent < 0){
        switch (myCurrentSpriteIndex){
            case 0:
                myCurrentSpriteIndex = 1;
            break;
            case 1:
                myCurrentSpriteIndex = 0;
            break;
            default:
                myCurrentSpriteIndex = 0;
            break;
        }
        mySpriteTimerCurrent += mySpriteTimerMax;
    }
    return true;
}

void Character::Render(SDL_Renderer* aRenderer){
    mySprites[myCurrentSpriteIndex]->Render(aRenderer);
}