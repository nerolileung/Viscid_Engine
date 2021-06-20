#ifndef SCENELEVEL_H
#define SCENELEVEL_H

#include "Scene.h"
#include "HatQuest.h"
#include "Character.h"
#include "TilePooler.h"
#include "framework/UI_Button.h"
#include <vector>
#include <array>

class SceneLevel : public Scene {
public:
    SceneLevel();
    ~SceneLevel();
    bool Init(SDL_Renderer* aRenderer) override;
    bool Update(float deltaTime) override;
    void Render(SDL_Renderer* aRenderer) override;
    HatQuest::SCENES GetNextScene(float& duration);
private:
    bool myPaused;
    bool myPauseKeyPressed;
    UI_Element* myPauseOverlay;
    UI_Button* myRestartButton;
    UI_Button* myQuitButton;
    UI_Button* myMainMenuButton;

    UI_Element* myControlInfo[3];
    int myControlInfoIndex;
    float myControlInfoTimer;
    void UpdateTutorial(float deltaTime);

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
    float myDuration;
};

#endif // SCENELEVEL_H