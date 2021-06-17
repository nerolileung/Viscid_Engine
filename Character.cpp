#include "Character.h"
#include "framework/Game.h"
#include "framework/Collisions.h"
#include <cmath>

Character::Character(){
    myCurrentSpriteIndex = 0;
    mySpriteTimerCurrent = 0.f;
}

Character::~Character(){
    tilePooler = nullptr;
}

bool Character::Init(SDL_Renderer* aRenderer, int unitSize, TilePooler* aTilePooler){
    gameUnit = unitSize;
    // initial position
    myPosition.x = gameUnit * 2;
    myPosition.y = gameUnit * 8;
    
    if (!InitSprites(aRenderer)) return false;
    ChangeState(PLAYER_STATE::RUNNING);

    tilePooler = aTilePooler;
    if (tilePooler == nullptr) return false;

    return true;
}

bool Character::InitSprites(SDL_Renderer* aRenderer){
    // running sprites
    mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element("data/player_1.png", aRenderer, myPosition.x, myPosition.y)));
    if (!mySprites[0]) return false;
    // use first sprite to determine scale of others
    float scale = gameUnit / mySprites[0]->GetDimensions().w;
    myPosition.w = gameUnit;
    mySprites[0]->SetSize({myPosition.w, myPosition.h},UI_Element::ASPECT_RATIO::WIDTH);

    mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element("data/player_2.png", aRenderer, myPosition.x, myPosition.y)));
    if (!mySprites[1]) return false;
    //myPosition.w = mySprites[1]->GetDimensions().w * scale;
    mySprites[1]->SetSize({myPosition.w, myPosition.h},UI_Element::ASPECT_RATIO::WIDTH);

    // todo other sprites; may need to adjust positions
    mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element("data/player_1.png", aRenderer, myPosition, UI_Element::ASPECT_RATIO::WIDTH)));
    if (!mySprites[2]) return false;
    mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element("data/player_2.png", aRenderer, myPosition, UI_Element::ASPECT_RATIO::WIDTH)));
    if (!mySprites[3]) return false;
    mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element("data/player_1.png", aRenderer, myPosition, UI_Element::ASPECT_RATIO::WIDTH)));
    if (!mySprites[4]) return false;
    mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element("data/player_2.png", aRenderer, myPosition, UI_Element::ASPECT_RATIO::WIDTH)));
    if (!mySprites[5]) return false;
    mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element("data/player_1.png", aRenderer, myPosition, UI_Element::ASPECT_RATIO::WIDTH)));
    if (!mySprites[6]) return false;
    mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element("data/player_2.png", aRenderer, myPosition, UI_Element::ASPECT_RATIO::WIDTH)));
    if (!mySprites[7]) return false;

    // return to original position
    myPosition = mySprites[0]->GetDimensions();
    myPosition.x = (gameUnit * 2) - (myPosition.w / 2);
    myPosition.y = (gameUnit * 7) - (myPosition.h / 2);

    return true;
}

void Character::Update(float deltaTime, float speed){
    // sprite animation
    mySpriteTimerMax = speed/3;
    if (mySpriteTimerMax < 0.05f) mySpriteTimerMax = 0.05f;

    mySpriteTimerCurrent -= deltaTime;
    if (mySpriteTimerCurrent < 0){
        if (myCurrentSpriteIndex % 2 == 0) myCurrentSpriteIndex++;
        else myCurrentSpriteIndex--;
        mySpriteTimerCurrent += mySpriteTimerMax;
    }

    if (myState != PLAYER_STATE::DEAD){
        // handle input-related state changes
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if ((keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
            && myState != PLAYER_STATE::JUMPING)
        {
            ChangeState(PLAYER_STATE::JUMPING);
        }
        else if ((keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
            && (myState == PLAYER_STATE::JUMPING || myState == PLAYER_STATE::RUNNING))
        {
            ChangeState(PLAYER_STATE::SLIDING);
        }
        else if ((!keystate[SDL_SCANCODE_DOWN] || !keystate[SDL_SCANCODE_S])
            && myState == PLAYER_STATE::SLIDING)
        {
            ChangeState(PLAYER_STATE::RUNNING);
        }

        // movement and related state changes
        UpdatePosition(deltaTime, speed);
        if (isDead()) ChangeState(PLAYER_STATE::DEAD);
    }
}

void Character::UpdatePosition(float deltaTime, float speed){
    if (myState == PLAYER_STATE::JUMPING){
        // steal hatquest logic
    }
    else {
        // fake gravity: a constant force
        myPosition.y += std::ceilf(deltaTime * gameUnit * 2);
        // correct for over-movement into tiles
        std::vector<Tile*> tiles = tilePooler->GetTilesCollidingWith(myPosition);
        if (!tiles.empty()){
            for (int i = 0; i < tiles.size(); i++){
            // only consider tiles below bottom edge of player
                if (tiles[i]->GetPosition().y > (myPosition.y - (myPosition.h/2))){
                    // check that we're still colliding with this tile
                    SDL_Rect centeredPosition = tiles[i]->GetPosition();
                    centeredPosition.x += (centeredPosition.w / 2);
                    centeredPosition.y += (centeredPosition.h / 2);
                    if (Collisions::Box(myPosition,centeredPosition)){
                        // push player on top of tile
                        myPosition.y = tiles[i]->GetPosition().y - (myPosition.h / 2);
                    }
                }
            }
        }
    }
    mySprites[myCurrentSpriteIndex]->SetPosition({myPosition.x,myPosition.y});
}

void Character::Render(SDL_Renderer* aRenderer){
    mySprites[myCurrentSpriteIndex]->Render(aRenderer);
}

void Character::ChangeState(PLAYER_STATE aState){
    myCurrentSpriteIndex = aState;
    myPosition = mySprites[myCurrentSpriteIndex]->GetDimensions();
    switch (aState){
        case PLAYER_STATE::RUNNING:
            // adjust collision box; sprite is unaffected
            myPosition.w *= 0.8f;
        break;
        case PLAYER_STATE::JUMPING:
        break;
        case PLAYER_STATE::SLIDING:
        break;
        case PLAYER_STATE::DEAD:
        break;
    }
    myState = aState;
}

bool Character::isDead(){
    // avoid redoing calculations
    if (myState == PLAYER_STATE::DEAD) return true;

    // fell
    if (myPosition.y > Game::WindowHeight - (myPosition.h/2))
        return true;

    // hit something
    bool hasCollided = false;
    std::vector<Tile*> tiles = tilePooler->GetTilesCollidingWith(myPosition);
    // ignore collisions with tiles below player (floor)
    if (!tiles.empty()){
        for (int i = 0; i < tiles.size(); i++){
            if (tiles[i]->GetPosition().y < (myPosition.y + (myPosition.h/2)))
                hasCollided = true;
        }
    }
    return hasCollided;
}