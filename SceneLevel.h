#ifndef SCENELEVEL_H
#define SCENELEVEL_H

#include "Scene.h"
#include "HatQuest.h"
#include "Character.h"
#include "TilePooler.h"
#include <vector>

class SceneLevel : public Scene {
public:
    SceneLevel();
    ~SceneLevel();
    bool Init(SDL_Renderer* aRenderer) override;
    bool Update(float deltaTime) override;
    void Render(SDL_Renderer* aRenderer) override;
    void SetPaused(bool paused) { myPaused = paused; };
    HatQuest::SCENES GetNextScene();
private:
    bool myPaused;
    Character* myPlayer;
    float myBackgroundColour[3];
    int myBackgroundColourChange[3];
    void UpdateBackgroundColour(float deltaTime);
    int myTileSize;
    int myTileMaxX;
    TilePooler* myTilePooler;
    bool myUpcomingTiles[3][8];
    std::vector<bool[8]> myUpcomingPiece;
    float myTileAdvanceCounter;
    void AdvanceTiles();
    void UpdateUpcomingTiles();
    float mySpeed;
};

#endif // SCENELEVEL_H