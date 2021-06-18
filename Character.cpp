#include "Character.h"
#include "framework/Game.h"
#include "framework/Collisions.h"
#include <cmath>
#include <cstring>

Character::Character(){
    myCurrentSpriteIndex = 0;
    mySpriteTimerCurrent = 0.f;
    myJumpTimerCurrent = 0.f;
}

Character::~Character(){
    tilePooler = nullptr;
}

bool Character::Init(SDL_Renderer* aRenderer, int unitSize, TilePooler* aTilePooler){
    gameUnit = unitSize;
    // initial position
    myPosition.x = gameUnit * 2;
    myPosition.y = gameUnit * 7;
    
    if (!InitSprites(aRenderer)) return false;
    ChangeState(PLAYER_STATE::RUNNING);

    tilePooler = aTilePooler;
    if (tilePooler == nullptr) return false;

    return true;
}

bool Character::InitSprites(SDL_Renderer* aRenderer){
    mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element("data/player_1.png", aRenderer, myPosition.x, myPosition.y)));
    // don't try to do math on an object that doesn't exist
    if (!mySprites[0]) return false;
    // use first sprite to determine scale of others
    float scale = gameUnit;
    scale /= mySprites[0]->GetDimensions().w;
    myPosition.w = scale * mySprites[0]->GetDimensions().w;
    mySprites[0]->SetSize({myPosition.w, myPosition.h},UI_Element::ASPECT_RATIO::WIDTH);

    // initialise all other sprites
    for (int i = 1; i < 8; i++){
        const char* filename;
        switch (i){ // todo probably include associated state in filename
            case 1: filename = "data/player_2.png"; break;
            case 2: filename = "data/player_3.png"; break;
            case 3: filename = "data/player_4.png"; break;
            case 4: filename = "data/player_5.png"; break;
            case 5: filename = "data/player_6.png"; break;
            case 6: filename = "data/player_7.png"; break;
            case 7: filename = "data/player_8.png"; break;
        } 
        mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element(filename, aRenderer, myPosition.x, myPosition.y)));
        if (!mySprites[i]) return false;
        myPosition.w = mySprites[i]->GetDimensions().w * scale;
        mySprites[i]->SetSize({myPosition.w, myPosition.h},UI_Element::ASPECT_RATIO::WIDTH);
        // todo may need to adjust positions to keep midpoint consistent
    }

    // return to original position
    myPosition = mySprites[0]->GetDimensions();
    myPosition.x = (gameUnit * 2) - (myPosition.w / 2);
    myPosition.y = (gameUnit * 7) - (myPosition.h / 2);
    mySprites[myCurrentSpriteIndex]->SetPosition({myPosition.x,myPosition.y});

    return true;
}

void Character::Update(float deltaTime, float speed){
    // sprite animation
    mySpriteTimerMax = 1 / (3 * speed);
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
            std::vector<Tile*> tilesBelow = tilePooler->GetTilesCollidingWith(myPosition);
            bool tileDirectlyBelow = false;
            if (!tilesBelow.empty()){
                for (int i = 0; i < tilesBelow.size(); i++){
                    int yPos = tilesBelow[i]->GetPosition().y;
                    if (yPos > myPosition.y && yPos < myPosition.y + gameUnit)
                        tileDirectlyBelow = true;
                }
            }
            if (tileDirectlyBelow) ChangeState(PLAYER_STATE::JUMPING);
        }
        else if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
        {
            if (myState != PLAYER_STATE::SLIDING)
                ChangeState(PLAYER_STATE::SLIDING);
        }
        else if (!keystate[SDL_SCANCODE_DOWN] || !keystate[SDL_SCANCODE_S])
        {
            if (myState == PLAYER_STATE::SLIDING)
                ChangeState(PLAYER_STATE::RUNNING);
        }
        
        UpdatePosition(deltaTime, speed);
        if (isDead()) ChangeState(PLAYER_STATE::DEAD);
    }
}

void Character::UpdatePosition(float deltaTime, float speed){
    if (myState == PLAYER_STATE::JUMPING && myJumpTimerCurrent > 0){
        myJumpTimerCurrent -= deltaTime;
        // go up for some time and stay up for a bit
        if (myJumpTimerCurrent > 0.8f){
            myPosition.y -= std::floorf(deltaTime * gameUnit * 5.f);
        }
    }
    else {
        // fake gravity: a constant force
        myPosition.y += std::ceilf(deltaTime * gameUnit * 6.f);
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
                        // landed
                        if (myState == PLAYER_STATE::JUMPING)
                            ChangeState(PLAYER_STATE::RUNNING);
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
    // find bottom corner (rendering position)
    SDL_Rect originalPosition = mySprites[myCurrentSpriteIndex]->GetDimensions();
    originalPosition.x = myPosition.x + (originalPosition.w / 2);
    originalPosition.y = myPosition.y + (originalPosition.h / 2);

    // update position
    myCurrentSpriteIndex = aState;
    myPosition = mySprites[myCurrentSpriteIndex]->GetDimensions();
    myPosition.x = originalPosition.x - (myPosition.w / 2);
    myPosition.y = originalPosition.y - (myPosition.h / 2);

    // update other logic
    if (aState == PLAYER_STATE::RUNNING){
        // allow player to fall down 1-wide holes; sprite is unaffected
        myPosition.w *= 0.8f;
    }
    else if (aState == PLAYER_STATE::JUMPING){
        myJumpTimerCurrent = myJumpTimerMax;
        // keep width of collision box similar to running sprite
        myPosition.w = mySprites[myCurrentSpriteIndex]->GetDimensions().w;
    }
    myState = aState;
}

bool Character::isDead(){
    // avoid redoing calculations
    if (myState == PLAYER_STATE::DEAD) return true;

    // fell
    if (myPosition.y + (myPosition.h/2) > Game::WindowHeight)
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