#include "Character.h"
#include "framework/Game.h"

Character::Character(){
    myCurrentSpriteIndex = 0;
    mySpriteTimerCurrent = 0.f;
}

Character::~Character(){
    tilePooler = nullptr;
}

bool Character::Init(SDL_Renderer* aRenderer, int unitSize, TilePooler* aTilePooler){
    gameUnit = unitSize;
    // running sprite dimensions
    myPosition.w = gameUnit;
    myPosition.h = gameUnit * 5 / 3;
    myPosition.x = (gameUnit * 2) - (myPosition.w / 2);
    myPosition.y = (gameUnit * 7) - (myPosition.h / 2);
    
    if (!InitSprites(aRenderer)) return false;
    ChangeState(PLAYER_STATE::RUNNING);

    tilePooler = aTilePooler;
    if (tilePooler == nullptr) return false;

    return true;
}

bool Character::InitSprites(SDL_Renderer* aRenderer){
    mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element("data/player_1.png", aRenderer, myPosition, UI_Element::ASPECT_RATIO::WIDTH)));
    if (!mySprites[0]) return false;
    mySprites.push_back(std::unique_ptr<UI_Element>(new UI_Element("data/player_2.png", aRenderer, myPosition, UI_Element::ASPECT_RATIO::WIDTH)));
    if (!mySprites[1]) return false;

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

void Character::UpdatePosition(float deltaTime, float speed){
    if (myState == PLAYER_STATE::JUMPING){
        // steal hatquest logic
    }
    else {
        // move into empty space below
        SDL_Point tilePosition = {myPosition.x, myPosition.y + myPosition.h / 2};
        tilePosition.x += myPosition.w/2;
        Tile* right = tilePooler->GetTileAt(tilePosition);
        tilePosition.x -= myPosition.w/2;
        Tile* middle = tilePooler->GetTileAt(tilePosition);
        tilePosition.x -= myPosition.w/2;
        Tile* left = tilePooler->GetTileAt(tilePosition);

        if (!right && !middle && !left){
            myPosition.y += deltaTime * speed;

            // correct for over-movement
            tilePosition.x = myPosition.x + myPosition.w/2;
            right = tilePooler->GetTileAt(tilePosition);
            tilePosition.x -= myPosition.w/2;
            middle = tilePooler->GetTileAt(tilePosition);
            tilePosition.x -= myPosition.w/2;
            left = tilePooler->GetTileAt(tilePosition);

            if (right || middle || left){
                if (right != nullptr)
                    myPosition.y = right->GetPosition().x;
                else if (middle != nullptr)
                    myPosition.y = middle->GetPosition().x;
                else if (left != nullptr)
                    myPosition.y = left->GetPosition().x;
                myPosition.y = myPosition.y - myPosition.h / 2;
            }
            mySprites[myCurrentSpriteIndex]->SetPosition({myPosition.x,myPosition.y});
        }
    }
}

void Character::Render(SDL_Renderer* aRenderer){
    mySprites[myCurrentSpriteIndex]->Render(aRenderer);
}

void Character::ChangeState(PLAYER_STATE aState){
    switch (aState){
        case PLAYER_STATE::RUNNING:
            myPosition = mySprites[0]->GetDimensions();
            myPosition.x = (gameUnit * 2) - (myPosition.w / 2);
            myPosition.y = (gameUnit * 7) - (myPosition.h / 2); // todo
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

bool Character::isDead(){
    // avoid redoing calculations
    if (myState == PLAYER_STATE::DEAD) return true;

    // fell
    if (myPosition.y > Game::WindowHeight - (myPosition.h/2))
        return true;

    // hit something
    bool hasCollided = false;
    // check tiles to the right and above
    for (int i = (myPosition.y - myPosition.h/2);
        i <= (myPosition.y + myPosition.h/2);
        i = i + gameUnit)
    {
        for (int j = (myPosition.x - myPosition.w/2);
            i <= (myPosition.x + myPosition.w/2);
            i = i + gameUnit)
        {
            if(tilePooler->GetTileAt({j,i}) != nullptr)
                hasCollided = true;
        }
    }
    return hasCollided;
}