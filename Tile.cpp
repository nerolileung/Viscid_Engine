#include "Tile.h"
#include <cmath>

int Tile::tileSize;

Tile::Tile(){
    myActive = false;
}

Tile::~Tile(){
    mySpritesheet = nullptr;
}

void Tile::Init(SDL_Point sourcePos, SDL_Point destPos, SDL_Texture* aSpritesheet){
    mySourceRect.h = 300;
    mySourceRect.w = 300;
    mySourceRect.x = sourcePos.x;
    mySourceRect.y = sourcePos.y;

    myPositionRect.h = tileSize;
    myPositionRect.w = tileSize;
    myPositionRect.x = destPos.x;
    myPositionRect.y = destPos.y;
    
    mySpritesheet = aSpritesheet;
    myActive = true;
};

void Tile::Update(float deltaTime, float speed){
    myPositionRect.x -= std::ceilf(deltaTime * speed * tileSize);
    // deactivate when offscreen
    if (myPositionRect.x < -myPositionRect.w)
        myActive = false;
}

void Tile::Render(SDL_Renderer* aRenderer){
    SDL_RenderCopy(aRenderer, mySpritesheet, &mySourceRect, &myPositionRect);
}