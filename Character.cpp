#include "Character.h"
#include "framework/Game.h"
#include "framework/Collisions.h"
#include "framework/AudioSystem.h"
#include <cmath>
#include <cstring>

Character::Character(){
    myCurrentSpriteIndex = 0;
    mySpriteTimerCurrent = 0.f;
    myJumpTimerCurrent = 0.f;
    myJumpForceCurrent = 0.f;
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
    
    myJumpLandSFX = AudioSystem::LoadClip("data/player_land_sfx.wav");
    mySlideSFX = AudioSystem::LoadClip("data/player_drop_sfx.wav");
    myDieSFX = AudioSystem::LoadClip("data/player_die_sfx.wav");

    // initialise state without triggering sfx
    myState = PLAYER_STATE::RUNNING;
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
    }

    // return to original position
    myPosition = mySprites[0]->GetDimensions();
    myPosition.x = (gameUnit * 2) - (myPosition.w / 2);
    myPosition.y = (gameUnit * 7) - (myPosition.h / 2);
    mySprites[myCurrentSpriteIndex]->SetPositionCentre({myPosition.x,myPosition.y});

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
        CheckCollisionDeath();

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
            if (myState != PLAYER_STATE::SLIDING && myState != PLAYER_STATE::SLIDING_FALLING){
                if (myState == PLAYER_STATE::JUMPING)
                    ChangeState(PLAYER_STATE::SLIDING_FALLING);
                else ChangeState(PLAYER_STATE::SLIDING);
            }
                
        }
        else if (!keystate[SDL_SCANCODE_DOWN] || !keystate[SDL_SCANCODE_S])
        {
            if (myState == PLAYER_STATE::SLIDING)
                ChangeState(PLAYER_STATE::RUNNING);
        }
        
        UpdatePosition(deltaTime, speed);
        CheckFallingDeath();
    }
}

void Character::UpdatePosition(float deltaTime, float speed){
    bool falling = true;
    if (myState == PLAYER_STATE::JUMPING && myJumpTimerCurrent > 0){
        myJumpTimerCurrent -= deltaTime;

        // timer controls how long the player spends going up + transition to falling
        float halfJumpTimerMax = myJumpTimerMax / 2;
        if (myJumpTimerCurrent > halfJumpTimerMax) falling = false;

        // jump force decreases on way up and increases on way down
        myJumpForceCurrent = myJumpForceMax * ((myJumpTimerCurrent - halfJumpTimerMax) / halfJumpTimerMax);

        myPosition.y -= std::floorf(deltaTime * gameUnit * myJumpForceCurrent);
    }
    else {
        // fake gravity: a constant force
        if (deltaTime < 0.01f)
            myPosition.y += std::ceilf(deltaTime * gameUnit * myJumpForceMax * 1.2f);
        // prevent player falling through floor due to lag
        else myPosition.y += std::ceilf(0.009f * gameUnit * myJumpForceMax * 1.2f);
    }
    if (falling){
        // correct for over-movement into tiles
        SDL_Rect checkPosition = myPosition;
        checkPosition.h = gameUnit;
        // check tiles below player
        checkPosition.y += (myPosition.h/2) + (gameUnit/2);
        std::vector<Tile*> tiles = tilePooler->GetTilesCollidingWith(checkPosition);
        // in midair
        if (tiles.empty() && myState == PLAYER_STATE::SLIDING)
            ChangeState(PLAYER_STATE::SLIDING_FALLING);
        else {
            int highestTileY = Game::WindowHeight;
            bool playerPushedOntoTile = false;
            
            for (int i = 0; i < tiles.size(); i++){
                if (myState == PLAYER_STATE::JUMPING || myState == PLAYER_STATE::SLIDING_FALLING){
                    // tile's y position should be close to bottom edge of player to count as landing
                    if (tiles[i]->GetPosition().y - (myPosition.y + (myPosition.h/2)) < 1){
                        switch (myState){
                            case PLAYER_STATE::JUMPING:
                                ChangeState(PLAYER_STATE::RUNNING);
                                // readjust collision box for checking
                                checkPosition = myPosition;
                                checkPosition.h = gameUnit;
                            break;
                            case PLAYER_STATE::SLIDING_FALLING:
                                ChangeState(PLAYER_STATE::SLIDING);
                            break;
                            default: break;
                        }
                    }
                    else continue;
                }
                // only stand on a tile above the next one
                if (tiles[i]->GetPosition().y < highestTileY){
                    // check that we're still colliding with this tile
                    checkPosition.y = myPosition.y + (myPosition.h/2) + (gameUnit/2);
                    SDL_Rect centeredPosition = tiles[i]->GetPosition();
                    centeredPosition.x += (centeredPosition.w / 2);
                    centeredPosition.y += (centeredPosition.h / 2);
                    
                    if (Collisions::Box(checkPosition,centeredPosition)){
                        // push player on top of tile; avoid snapping to it
                        if (tiles[i]->GetPosition().y - (myPosition.y + (myPosition.h/2)) < 1){
                            myPosition.y = tiles[i]->GetPosition().y - (myPosition.h/2);
                            playerPushedOntoTile = true;
                            if (myState == PLAYER_STATE::RUNNING_FALLING)
                                ChangeState(PLAYER_STATE::RUNNING);
                        }
                    }
                    highestTileY = tiles[i]->GetPosition().y;
                }
            }
            /*if (!playerPushedOntoTile && myState == PLAYER_STATE::RUNNING)
                ChangeState(PLAYER_STATE::RUNNING_FALLING);*/
        }
    }
    mySprites[myCurrentSpriteIndex]->SetPositionCentre({myPosition.x,myPosition.y});
}

void Character::Render(SDL_Renderer* aRenderer){
    mySprites[myCurrentSpriteIndex]->Render(aRenderer);
}

void Character::ChangeState(PLAYER_STATE aState){
    // find bottom corner (rendering position)
    SDL_Rect originalPosition = mySprites[myCurrentSpriteIndex]->GetDimensions();
    originalPosition.x = myPosition.x + (originalPosition.w / 2);
    originalPosition.y = myPosition.y + (originalPosition.h / 2);

    // update position to new midpoint
    // don't change sprites between running/sliding and their falling versions
    if ((aState == PLAYER_STATE::RUNNING_FALLING && myState != PLAYER_STATE::RUNNING)
        || (aState == PLAYER_STATE::RUNNING && myState != PLAYER_STATE::RUNNING_FALLING))
        myCurrentSpriteIndex = PLAYER_STATE::RUNNING;
    else if ((aState == PLAYER_STATE::SLIDING_FALLING && myState != PLAYER_STATE::SLIDING)
        || (aState == PLAYER_STATE::SLIDING && myState != PLAYER_STATE::SLIDING_FALLING))
        myCurrentSpriteIndex = PLAYER_STATE::SLIDING;
    else myCurrentSpriteIndex = aState;
    myPosition = mySprites[myCurrentSpriteIndex]->GetDimensions();
    myPosition.x = originalPosition.x - (myPosition.w / 2);
    myPosition.y = originalPosition.y - (myPosition.h / 2);

    // update other logic
    switch (aState){
        case PLAYER_STATE::RUNNING:
            // resize hitbox, allow player to fall down 1-wide holes; sprite is unaffected
            myPosition.w *= 0.75f;

            // sfx
            if (myState == PLAYER_STATE::JUMPING || myState == PLAYER_STATE::RUNNING_FALLING)
                AudioSystem::PlayClip(myJumpLandSFX);
        break;
        case PLAYER_STATE::JUMPING:
            myJumpTimerCurrent = myJumpTimerMax;
            myJumpForceCurrent = myJumpForceMax;
            // keep width of collision box similar to running sprite
            myPosition.w = mySprites[myCurrentSpriteIndex]->GetDimensions().w;
        break;
        case PLAYER_STATE::SLIDING:
            // play sfx only when hitting the ground
            if (myState == PLAYER_STATE::SLIDING_FALLING || myState == PLAYER_STATE::RUNNING)
                AudioSystem::PlayClip(mySlideSFX);
        break;
        case PLAYER_STATE::DEAD:
            AudioSystem::PlayClip(myDieSFX);
        break;
        default: break;
    }
    myState = aState;
}

void Character::CheckCollisionDeath(){
    // hit something
    std::vector<Tile*> tiles = tilePooler->GetTilesCollidingWith(myPosition);
    // ignore collisions with tiles below player (floor)
    if (!tiles.empty()){
        for (int i = 0; i < tiles.size(); i++){
            if (tiles[i]->GetPosition().y < myPosition.y)
                ChangeState(PLAYER_STATE::DEAD);
        }
    }
}

void Character::CheckFallingDeath(){
    // fell
    if (myPosition.y + (myPosition.h/2) > Game::WindowHeight)
        ChangeState(PLAYER_STATE::DEAD);
}