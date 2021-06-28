#include "SceneLevel.h"
#include <cmath>
#include "TilePatterns.h"

SceneLevel::SceneLevel(){
    // level flags
    myFinished = false;
    myPaused = false;
    myPauseKeyPressed = false;

    // tutorial info
    myControlInfoIndex = 0;
    myControlInfoTimer = 3.f;

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
    for (int i = 0; i < 3; i++)
        myUpcomingTiles[i] = TilePatterns::GetPattern(TilePatterns::PATTERNS::LOW_FLOOR_REPEAT);

    myTileAdvanceCounter = 0.f;
    mySpeed = 0.5f;
    myDuration = 0.f;

    myHatTimerMax = 1 / (3 * mySpeed);
    myHatTimerCurrent = myHatTimerMax;
    myHatDirection = 1;

    // initialise pointers to other objects
    myPlayer = new Character();
    myTilePooler = new TilePooler();
}

SceneLevel::~SceneLevel(){
    delete myPlayer;
    myPlayer = nullptr;

    delete myTilePooler;
    myTilePooler = nullptr;

    delete myPauseOverlay;
    myPauseOverlay = nullptr;

    delete myRestartButton;
    myRestartButton = nullptr;
    delete myQuitButton;
    myQuitButton = nullptr;
    delete myMainMenuButton;
    myMainMenuButton = nullptr;

    if (myControlInfoIndex > 0){
        delete myControlInfo[0];
        myControlInfo[0] = nullptr;
        delete myControlInfo[1];
        myControlInfo[1] = nullptr;
        delete myControlInfo[2];
        myControlInfo[2] = nullptr;
    }

    delete myHat;
    myHat = nullptr;
}

bool SceneLevel::Init(SDL_Renderer* aRenderer, bool playTutorial){
    // get some extra precision
    float maximumNumberOfTilesOnScreen = (float)Game::WindowWidth/(float)myTileSize;
    myTileMaxX = (int)std::ceilf(maximumNumberOfTilesOnScreen);

    myTilePooler->Init(aRenderer, myTileSize, myTileMaxX);
    if (myTilePooler == nullptr) return false;

    myPlayer->Init(aRenderer, myTileSize, myTilePooler);
    if (myPlayer == nullptr) return false;

    for (int i = 0; i < myTileMaxX; i++){
        myTilePooler->SetFreeTile({i*myTileSize, 7*myTileSize},{0,900});
    }

    // pause menu
    myPauseOverlay = new UI_Element("data/pause_bg.png",aRenderer);
    if (myPauseOverlay == nullptr) return false;
    // todo different buttons?
    SDL_Rect pauseButtonPosition = {(int)(Game::WindowWidth*0.1f + myTileSize), (int)(Game::WindowHeight*0.5f), myTileSize*2, myTileSize*2};
    myRestartButton = new UI_Button("data/mainmenu_button.png"," Restart ",gameFonts[1],gameFontColours[0],aRenderer,pauseButtonPosition,UI_Element::ASPECT_RATIO::NONE);
    if (myRestartButton == nullptr) return false;

    pauseButtonPosition.x = Game::WindowWidth*0.5f;
    myQuitButton = new UI_Button("data/mainmenu_button.png","  Quit  ",gameFonts[1],gameFontColours[0],aRenderer,pauseButtonPosition,UI_Element::ASPECT_RATIO::NONE);
    if (myQuitButton == nullptr) return false;

    pauseButtonPosition.x = Game::WindowWidth*0.9f - myTileSize;
    myMainMenuButton = new UI_Button("data/mainmenu_button.png","Main Menu",gameFonts[1],gameFontColours[0],aRenderer,pauseButtonPosition,UI_Element::ASPECT_RATIO::NONE);
    if (myMainMenuButton == nullptr) return false;
    
    if (playTutorial){
        // controls tutorial overlay
        SDL_Rect tutorialPosition = {(int)(Game::WindowWidth*0.5f),(int)(Game::WindowHeight*0.4f),(int)(Game::WindowHeight*0.5f),(int)(Game::WindowHeight*0.5f)};
        myControlInfo[0] = new UI_Element("data/tutorial_pause.png",aRenderer,tutorialPosition,UI_Element::ASPECT_RATIO::WIDTH);
        if (myControlInfo[0] == nullptr) return false;
        myControlInfo[1] = new UI_Element("data/tutorial_jump.png",aRenderer,tutorialPosition,UI_Element::ASPECT_RATIO::WIDTH);
        if (myControlInfo[1] == nullptr) return false;
        myControlInfo[2] = new UI_Element("data/tutorial_slide.png",aRenderer,tutorialPosition,UI_Element::ASPECT_RATIO::WIDTH);
        if (myControlInfo[2] == nullptr) return false;
    }
    else {
        myControlInfoIndex = -1;
        mySpeed = 1.f;
    }

    SDL_Rect hatPosition = {Game::WindowWidth - myTileSize,(Game::WindowHeight/2),(int)(myTileSize*0.8),(int)(myTileSize*0.8)};
    myHat = new UI_Element("data/hat.png", aRenderer, hatPosition, UI_Element::ASPECT_RATIO::NONE);
    if (myHat == nullptr) return false;

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
        myRestartButton->Update(deltaTime);
        myQuitButton->Update(deltaTime);
        myMainMenuButton->Update(deltaTime);
        if (myMainMenuButton->isClicked())
            myFinished = true;
        else if (myQuitButton->isClicked())
            return false;
        else if (myRestartButton->isClicked()){
            myPaused = false;
            myFinished = true;
        }
    }
    else {
        myDuration += deltaTime;

        UpdateBackgroundColour(deltaTime);

        // display control scheme infographics at the start
        if (myControlInfoIndex < 3 && myControlInfoIndex != -1)
            UpdateTutorial(deltaTime);
        
        // generate more map and allocate next tile
        myTileAdvanceCounter -= std::ceilf(deltaTime * mySpeed * myTileSize);
        if (myTileAdvanceCounter < 0){
            UpdateUpcomingTiles();
            AdvanceTiles();
            myTileAdvanceCounter += myTileSize;
        } 

        myTilePooler->Update(deltaTime, mySpeed);
        myPlayer->Update(deltaTime, mySpeed);

        UpdateHatPosition(deltaTime);

        // start speeding up after tutorial
        if (myControlInfoIndex > 2 || myControlInfoIndex < 0)
            mySpeed += (deltaTime * 0.01f);
            
        myFinished = myPlayer->isDead(); // todo start delay to play ghost animation
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
    // shift tile records
    myUpcomingTiles[0] = myUpcomingTiles[1];
    myUpcomingTiles[1] = myUpcomingTiles[2];

    if (myUpcomingPattern.empty()){
        // get a new pattern
        int tileRepeatCount = std::floorf(mySpeed);
        int patternIndex = std::rand() % TilePatterns::size();
        patternIndex = patternIndex - (patternIndex % 3);
        myUpcomingPattern = TilePatterns::GetPattern((TilePatterns::PATTERNS)patternIndex, tileRepeatCount);
    }

    // add buffer between sliding and jumping to a higher level
    if (
        myUpcomingTiles[0] == TilePatterns::GetPattern(TilePatterns::PATTERNS::LOW_SLIDE_REPEAT) &&
        myUpcomingTiles[1] == TilePatterns::GetPattern(TilePatterns::PATTERNS::LOW_SLIDE_END) &&
        myUpcomingPattern[0] == TilePatterns::GetPattern(TilePatterns::PATTERNS::MID_FLOOR_START)
        /*|| (myUpcomingTiles[0] == TilePatterns::GetPattern(TilePatterns::PATTERNS::MID_SLIDE_REPEAT) &&
        myUpcomingTiles[1] == TilePatterns::GetPattern(TilePatterns::PATTERNS::MID_SLIDE_END) &&
        myUpcomingPattern[0] == TilePatterns::GetPattern(TilePatterns::PATTERNS::HIGH_FLOOR_LOW_FLOOR_START))*/
    )
    {
        myUpcomingTiles[2] = myUpcomingTiles[1];
    }
    else {
        myUpcomingTiles[2] = myUpcomingPattern[0];
        if (myUpcomingPattern.size() == 1) myUpcomingPattern.pop_back();
        else myUpcomingPattern.erase(myUpcomingPattern.begin(),myUpcomingPattern.begin()+1);
    }
}

void SceneLevel::AdvanceTiles(){
    for (int i = 0; i < 8; i++){
        // is there a tile here?
        int FlagToCheck = (1 << i);
        if ((myUpcomingTiles[1] & FlagToCheck) == 0) continue;

        // determine sprite based on adjacent tiles
        int UpperFlagToCheck = (1 << (i-1));
        int LowerFlagToCheck = (1 << (i+1));

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
        SDL_Point spritePosition = { 300 * (index % 4), 300 * ((index - (index % 4)) / 4)};
        myTilePooler->SetFreeTile({myTileMaxX*myTileSize, i*myTileSize},spritePosition);
    }
}

void SceneLevel::UpdateTutorial(float deltaTime){
    myControlInfoTimer -= deltaTime;
    if (myControlInfoTimer < 0){
        myControlInfoIndex++;
        myControlInfoTimer = 3.f;
        if (myControlInfoIndex > 2) mySpeed = 1.f;
    }
}

void SceneLevel::UpdateHatPosition(float deltaTime){
    SDL_Point hatPosition;
    hatPosition.x = Game::WindowWidth - myTileSize;

    myHatTimerMax = 1 / (3 * mySpeed);
    if (myHatTimerMax < 0.05f) myHatTimerMax = 0.05f;

    myHatTimerCurrent -= deltaTime;
    if (myHatTimerCurrent < 0){
        myHatDirection *= -1;
        myHatTimerCurrent = myHatTimerMax;
    }

    hatPosition.y = (Game::WindowHeight/2) + (myTileSize*0.1f*myHatDirection);
    myHat->SetPosition(hatPosition);
}

void SceneLevel::Render(SDL_Renderer* aRenderer){
    // background changes colour
    SDL_SetRenderDrawColor(aRenderer, myBackgroundColour[0], myBackgroundColour[1], myBackgroundColour[2], SDL_ALPHA_OPAQUE);
    SDL_RenderClear(aRenderer);

    // render regular screen
    myTilePooler->Render(aRenderer);
    myPlayer->Render(aRenderer);
    myHat->Render(aRenderer);

    // render tutorial
    if (myControlInfoIndex < 3 && myControlInfoIndex != -1)
        myControlInfo[myControlInfoIndex]->Render(aRenderer);

    if (myPaused){
        // render pause menu ui on top
        myPauseOverlay->Render(aRenderer);
        myRestartButton->Render(aRenderer);
        myQuitButton->Render(aRenderer);
        myMainMenuButton->Render(aRenderer);
    }
}

HatQuest::SCENES SceneLevel::GetNextScene(float& duration){
    duration = myDuration;

    if (myPaused) return HatQuest::SCENES::MAIN_MENU;
    if (myPlayer->isDead()) return HatQuest::SCENES::END;
    else return HatQuest::SCENES::PLAYING;
}