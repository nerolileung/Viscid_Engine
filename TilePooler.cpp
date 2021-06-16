#include "TilePooler.h"
#include "framework/Game.h"
#include <cmath>
#include "framework/include/SDL2/SDL_image.h"

TilePooler::TilePooler(){
}

TilePooler::~TilePooler(){
    myTiles.clear();
    for (int i = 0; i < sizeof(myTileSprites); i++){
        SDL_DestroyTexture(myTileSprites[i]);
        myTileSprites[i] = nullptr;
    }
}

bool TilePooler::Init(SDL_Renderer* aRenderer, int tileSize, int maxTileWidth){
    // load tiles
    myPoolSize = tileSize*maxTileWidth;
    myTiles.resize(myPoolSize);

    for (int i = 0; i < myPoolSize; i++){
        myTiles.push_back(Tile());
    }
    Tile::SetSize(tileSize);
    
    // load tile sprites
    SDL_Surface* tempSurface = IMG_Load("data/tile.png");
    myTileSprites[0] = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
    // todo load others
    myTileSprites[1] = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
    myTileSprites[2] = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
    SDL_FreeSurface(tempSurface);
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