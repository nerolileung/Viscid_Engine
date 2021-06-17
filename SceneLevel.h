#ifndef SCENELEVEL_H
#define SCENELEVEL_H

#include "Scene.h"
#include "HatQuest.h"
#include "Character.h"
#include "TilePooler.h"
#include <vector>
#include <array>

class SceneLevel : public Scene {
public:
    SceneLevel();
    ~SceneLevel();
    bool Init(SDL_Renderer* aRenderer) override;
    bool Update(float deltaTime) override;
    void Render(SDL_Renderer* aRenderer) override;
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
    std::array<unsigned char,3> myUpcomingTiles;
    std::vector<unsigned char> myUpcomingPattern;
    float myTileAdvanceCounter;
    void AdvanceTiles();
    void UpdateUpcomingTiles();
    float mySpeed;
};

#endif // SCENELEVEL_H