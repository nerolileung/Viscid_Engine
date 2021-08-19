#ifndef SCENELEVEL_H
#define SCENELEVEL_H

#include "Scene.h"
#include "Character.h"
#include "TilePooler.h"
#include "framework/UI_Button.h"
#include <vector>
#include <array>
#include "framework/include/SDL2/SDL_mixer.h"

class SceneLevel : public Scene {
public:
    SceneLevel();
    ~SceneLevel();
    bool Init(SDL_Renderer* aRenderer) override { return false; };
    bool Init(SDL_Renderer* aRenderer, bool playTutorial);
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

    UI_Element* myHat;
    float myHatTimerCurrent;
    float myHatTimerMax;
    int myHatDirection;
    int myHatTargetY;
    void UpdateHatPosition(float deltaTime);

    float myBackgroundColourHSV[3];
    float myBackgroundColourRGB[3];
    int myBackgroundColourChange;
    void UpdateBackgroundColour(float deltaTime);
    void BackgroundColourHSVToRGB();

    int myBackgroundMusicClips[4][4];
    void UpdateBackgroundMusic();

    int myTileSize;
    int myTileMaxX;
    TilePooler* myTilePooler;
    std::array<unsigned char*,3> myUpcomingTiles;
    std::vector<unsigned char*> myUpcomingPattern;
    int myTileAdvanceCounter;
    void AdvanceTiles(int offset);
    void UpdateUpcomingTiles();
    
    float mySpeed;
    float myDuration;
};

#endif // SCENELEVEL_H