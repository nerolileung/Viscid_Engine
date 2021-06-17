#ifndef TILE_H
#define TILE_H

#include "framework/include/SDL2/SDL.h"

class Tile {
public:
    Tile();
    ~Tile();
    void Init(SDL_Point sourcePos, SDL_Point destPos, SDL_Texture* aSpritesheet);
    void Update(float deltaTime, float speed);
    void Render(SDL_Renderer* aRenderer);
    bool isActive() { return myActive; };
    SDL_Rect GetPosition(){ return myPositionRect; };
    static void SetSize(int aSize) { tileSize = aSize; };
private:
    SDL_Rect mySourceRect;
    SDL_Rect myPositionRect;
    SDL_Texture* mySpritesheet;
    bool myActive;
    static int tileSize;
};
#endif //TILE_H