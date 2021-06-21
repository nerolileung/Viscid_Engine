#ifndef TILEPOOLER_H
#define TILEPOOLER_H

#include "framework/include/SDL2/SDL.h"
#include "Tile.h"
#include <vector>

class TilePooler {
public:
    TilePooler();
    ~TilePooler();
    void Update(float deltaTime, float speed);
    void Render(SDL_Renderer* aRenderer);
    bool Init(SDL_Renderer* aRenderer, int tileSize, int maxTileWidth);
    bool SetFreeTile(SDL_Point aPosition, SDL_Point aSprite);
    std::vector<Tile*> GetTilesCollidingWith(SDL_Rect aPosition);
private:
    int myPoolSize;
    std::vector<Tile> myTiles;
    SDL_Texture* myTileSprites[3];
};

#endif //TILEPOOLER_H