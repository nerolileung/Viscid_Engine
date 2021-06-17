#include "TilePooler.h"
#include "framework/Game.h"
#include <cmath>
#include "framework/include/SDL2/SDL_image.h"
#include "framework/Collisions.h"

TilePooler::TilePooler(){
}

TilePooler::~TilePooler(){
    myTiles.clear();
    // todo switch this to a std::array if more sheets are added
    SDL_DestroyTexture(myTileSprites[0]);
    myTileSprites[0] = nullptr;
    SDL_DestroyTexture(myTileSprites[1]);
    myTileSprites[1] = nullptr;
    SDL_DestroyTexture(myTileSprites[2]);
    myTileSprites[2] = nullptr;
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
    SDL_Surface* tempSurface = IMG_Load("data/tiles_1.png");
    myTileSprites[0] = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
    SDL_FreeSurface(tempSurface);
    tempSurface = IMG_Load("data/tiles_2.png");
    myTileSprites[1] = SDL_CreateTextureFromSurface(aRenderer,tempSurface);
    SDL_FreeSurface(tempSurface);
    tempSurface = IMG_Load("data/tiles_3.png");
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

std::vector<Tile*> TilePooler::GetTilesCollidingWith(SDL_Rect aPosition){
    std::vector<Tile*> collidingTiles;
    for (int i = 0; i < myTiles.size(); i++){
        if (myTiles[i].isActive()){
            SDL_Rect centeredPosition = myTiles[i].GetPosition();
            centeredPosition.x += (centeredPosition.w / 2);
            centeredPosition.y += (centeredPosition.h / 2);
            if (Collisions::Box(aPosition, centeredPosition))
                collidingTiles.push_back(&myTiles[i]);
        }
    }
    return collidingTiles;
}