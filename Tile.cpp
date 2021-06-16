#include "Tile.h"

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
    myPositionRect.x -= deltaTime * speed;
    // deactivate when offscreen
    if (myPositionRect.x + (myPositionRect.w / 2) < 0)
        myActive = false;
}

void Tile::Render(SDL_Renderer* aRenderer){
    SDL_RenderCopy(aRenderer, mySpritesheet, &mySourceRect, &myPositionRect);
}

bool Tile::operator == (Tile otherTile){
    if (mySourceRect.x != otherTile.mySourceRect.x) return false;
    if (mySourceRect.y != otherTile.mySourceRect.y) return false;
    if (mySpritesheet != otherTile.mySpritesheet) return false;
    if (myPositionRect.x != otherTile.myPositionRect.x) return false;
    if (myPositionRect.y != otherTile.myPositionRect.y) return false;
    if (myActive != otherTile.myActive) return false;
    return true;
}