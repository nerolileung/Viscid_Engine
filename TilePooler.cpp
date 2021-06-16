#include "TilePooler.h"
#include "framework/Game.h"
#include <cmath>

TilePooler::TilePooler(){
}

TilePooler::~TilePooler(){
    myTiles.clear();
    for (int i = 0; i < sizeof(myTileSprites); i++){
        SDL_DestroyTexture(myTileSprites[i]);
        myTileSprites[i] = nullptr;
    }
}

bool TilePooler::Init(SDL_Renderer* aRenderer, int tileSize){
    int maxTileWidth = (int)std::ceil(Game::WindowWidth/tileSize);
    myPoolSize = tileSize*maxTileWidth;
    myTiles.resize(myPoolSize);

    for (int i = 0; i < myPoolSize; i++){
        myTiles.push_back(Tile());
    }
    Tile::SetSize(tileSize);
    
    //load sprite sheets
    for (int i = 0; i < sizeof(myTileSprites); i++)
        if (myTileSprites[i] == nullptr) return false;
    
    return true;
}

void TilePooler::Update(float deltaTime, float speed){
    for (int i = 0; i < myTiles.size(); i++){
        if (myTiles[i].isActive())
            myTiles[i].Update(deltaTime, speed);
    }
}

void TilePooler::Render(SDL_Renderer* aRenderer){
    for (int i = 0; i < myTiles.size(); i++){
        if (myTiles[i].isActive())
            myTiles[i].Render(aRenderer);
    }
}

bool TilePooler::SetFreeTile(SDL_Point aPosition, SDL_Point aSprite){
    for (int i = 0; i < myTiles.size(); i++){
        if (!myTiles[i].isActive()){
            myTiles[i].Init(aSprite, aPosition, myTileSprites[std::rand()%3]);
            return true;
        }
    }
    return false;
}

Tile* TilePooler::GetTileAt(SDL_Point aPosition){
    for (int i = 0; i < myTiles.size(); i++){
        SDL_Rect tileRect = myTiles[i].GetPosition();
        if (SDL_PointInRect(&aPosition, &tileRect)){
            return &myTiles[i];
        }
    }
    return nullptr;
}