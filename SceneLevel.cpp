#include "SceneLevel.h"
#include <cmath>
#include "TilePatterns.h"

SceneLevel::SceneLevel(){
    // level flags
    myFinished = false;
    myPaused = false;
    myPauseKeyPressed = false;

    // initialise background colour
    myBackgroundColour[0] = 28.f - std::rand()%4;
    myBackgroundColour[1] = 150.f + std::rand()%20;
    myBackgroundColour[2] = 201.f - std::rand()%50;
    myBackgroundColourChange[0] = 5;
    myBackgroundColourChange[1] = 10;
    myBackgroundColourChange[2] = -10;

    // initialise map
    myTileSize = Game::WindowHeight/8; // 8 tiles up, 9-32 tiles across (avg. 14)
    TilePatterns::Init();
    for (int i = 0; i < 3; i++){
        myUpcomingTiles[i] = TilePatterns::GetRow(7);
    }
    myUpcomingPattern = TilePatterns::GetPattern(TilePatterns::PATTERNS::LOW_FLOOR);

    myTileAdvanceCounter = myTileSize;
    mySpeed = 1.f;

    // initialise pointers to other objects
    myPlayer = new Character();
    myTilePooler = new TilePooler();
}

SceneLevel::~SceneLevel(){
    delete myPlayer;
    myPlayer = nullptr;

    delete myTilePooler;
    myTilePooler = nullptr;
}

bool SceneLevel::Init(SDL_Renderer* aRenderer){
    myTileMaxX = (int)std::ceil(Game::WindowWidth/myTileSize);

    myTilePooler->Init(aRenderer, myTileSize, myTileMaxX);
    if (myTilePooler == nullptr) return false;

    myPlayer->Init(aRenderer, myTileSize, myTilePooler);
    if (myPlayer == nullptr) return false;

    for (int i = 0; i <= myTileMaxX; i++){
        myTilePooler->SetFreeTile({i*myTileSize, 7*myTileSize},{0,0});
    }

    // pause menu
    myPauseOverlay = new UI_Element("data/pause_bg.png",aRenderer);

    return true;
}

bool SceneLevel::Update(float deltaTime){
    // pause game on keydown
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (!myPaused && keystate[SDL_SCANCODE_ESCAPE] && !myPauseKeyPressed)
        myPaused = true;
    else if (myPaused && keystate[SDL_SCANCODE_ESCAPE] && !myPauseKeyPressed)
        myPaused = false;
    myPauseKeyPressed = keystate[SDL_SCANCODE_ESCAPE];

    if (myPaused){
        // update pause menu ui and nothing else
        myPauseOverlay->Update(deltaTime);
        // if return to main menu button is pressed, set finished
        // if quit button is pressed, return false
        // if restart button is pressed, set finished and unpause
    }
    else {
        UpdateBackgroundColour(deltaTime);
        
        // generate more map and allocate tiles
        myTileAdvanceCounter -= std::ceilf(mySpeed * deltaTime);
        if (myTileAdvanceCounter < 0){
            UpdateUpcomingTiles();
            AdvanceTiles();
            myTileAdvanceCounter += myTileSize;
        } 

        myTilePooler->Update(deltaTime, mySpeed);
        myPlayer->Update(deltaTime, mySpeed);
        myFinished = myPlayer->isDead(); // todo start ghost animation
    }
    return true;
}

void SceneLevel::UpdateBackgroundColour(float deltaTime){
    myBackgroundColour[0] += myBackgroundColourChange[0] * deltaTime;
    if (myBackgroundColour[0] < 0) myBackgroundColour[0] = 0;
    if (myBackgroundColour[0] > 150) myBackgroundColour[0] = 150;

    myBackgroundColour[1] += myBackgroundColourChange[1] * deltaTime;
    if (myBackgroundColour[1] < 0) myBackgroundColour[1] = 0;

    myBackgroundColour[2] += myBackgroundColourChange[2] * deltaTime;
    if (myBackgroundColour[2] < 0) myBackgroundColour[2] = 0;
    
    // green to blue
    if (myBackgroundColour[1] >= 200){
        myBackgroundColourChange[0] = -5;
        myBackgroundColourChange[1] = -20;
        myBackgroundColourChange[2] = 20;
    }
    // blue to purple
    else if (myBackgroundColourChange[2] == 20 && myBackgroundColour[2] > 160){
        myBackgroundColourChange[0] = 30;
        myBackgroundColourChange[1] = -30;
        myBackgroundColourChange[2] = 15;
    }
    // purple to blue
    else if (myBackgroundColour[2] >= 200){
        myBackgroundColourChange[0] = -20;
        myBackgroundColourChange[1] = 10;
        myBackgroundColourChange[2] = -10;
    }
    // blue to green
    else if (myBackgroundColourChange[0] == -20 && myBackgroundColour[0] < 0){
        myBackgroundColourChange[0] = 5;
        myBackgroundColourChange[1] = 10;
        myBackgroundColourChange[2] = -10;
    }
}

void SceneLevel::UpdateUpcomingTiles(){
    myUpcomingTiles[0] = myUpcomingTiles[1];
    myUpcomingTiles[1] = myUpcomingTiles[2];

    if (myUpcomingPattern.empty()){
        int patternIndex = std::rand() % TilePatterns::size();
        // todo do cool logic combining pieces/doing certain amounts of low/mid/high pieces
        myUpcomingPattern = TilePatterns::GetPattern((TilePatterns::PATTERNS)patternIndex);
    }

    myUpcomingTiles[2] = myUpcomingPattern[0];
    if (myUpcomingPattern.size() == 1) myUpcomingPattern.pop_back();
    else myUpcomingPattern.erase(myUpcomingPattern.begin(),myUpcomingPattern.begin()+1);
}

void SceneLevel::AdvanceTiles(){
    for (int i = 0; i < 8; i++){
        // is there a tile here?
        int FlagToCheck = TilePatterns::GetRow(i);
        if ((myUpcomingTiles[1] & FlagToCheck) == 0) continue;

        // determine sprite based on adjacent tiles
        int UpperFlagToCheck = TilePatterns::GetRow(i-1);
        int LowerFlagToCheck = TilePatterns::GetRow(i+1);

        int index = 0;
        if ((myUpcomingTiles[0] & FlagToCheck) == 0) // left empty
            index += 1;
        if ((myUpcomingTiles[2] & FlagToCheck) == 0) // right empty
            index += 2;
        if (i == 0 || ((myUpcomingTiles[1] & UpperFlagToCheck) == 0)) // up
            index += 4;
        if (i == 7 || ((myUpcomingTiles[1] & LowerFlagToCheck) == 0)) // down
            index += 8;

        // put new tile at far right of the screen
        SDL_Point spritePosition = { myTileSize * (i % 4), myTileSize * ((i - (i % 4)) / 4)};
        myTilePooler->SetFreeTile({(myTileMaxX + 1)*myTileSize, i*myTileSize},spritePosition);
    }
}

void SceneLevel::Render(SDL_Renderer* aRenderer){
    // background changes colour
    SDL_SetRenderDrawColor(aRenderer, myBackgroundColour[0], myBackgroundColour[1], myBackgroundColour[2], SDL_ALPHA_OPAQUE);
    SDL_RenderClear(aRenderer);

    // render regular screen
    myTilePooler->Render(aRenderer);
    myPlayer->Render(aRenderer);

    if (myPaused){
        // render pause menu ui on top
        myPauseOverlay->Render(aRenderer);
    }
}

HatQuest::SCENES SceneLevel::GetNextScene(){
    if (myPaused) return HatQuest::SCENES::MAIN_MENU;
    if (myPlayer->isDead()) return HatQuest::SCENES::END;
    else return HatQuest::SCENES::PLAYING;
}