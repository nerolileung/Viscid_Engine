#include "SceneLevel.h"
#include "TilePatterns.h"
#include <cmath>
#include "framework/AudioSystem.h"

SceneLevel::SceneLevel(){
    // level flags
    myFinished = false;
    myPaused = false;
    myPauseKeyPressed = false;

    // tutorial info
    myControlInfoIndex = 0;
    myControlInfoTimer = 3.f;

    // initialise background colour
    myBackgroundColourHSV[0] = 120.f + std::rand()%50; // stick to greens
    myBackgroundColourHSV[1] = (50.f + std::rand()%40)/100;
    myBackgroundColourHSV[2] = (60.f + std::rand()%25)/100;
    myBackgroundColourChange = 1;
    BackgroundColourHSVToRGB();

    // initialise background music
    myBackgroundMusicBass = AudioSystem::LoadClip("data/level_bgm_bass_slow.wav");
    myBackgroundMusicClips[0] = AudioSystem::LoadClip("data/level_bgm_green_slow.wav");
    myBackgroundMusicClips[1] = AudioSystem::LoadClip("data/level_bgm_cyan_slow.wav");
    myBackgroundMusicClips[2] = AudioSystem::LoadClip("data/level_bgm_indigo_slow.wav");
    myBackgroundMusicClips[3] = AudioSystem::LoadClip("data/level_bgm_purple_slow.wav");
    
    // use channels 0 and 1 for level bgm's bass and melody
    AudioSystem::SetVolumeClips(AudioSystem::GetVolumeMusic(),0);
    AudioSystem::SetVolumeClips(AudioSystem::GetVolumeMusic(),1);
    AudioSystem::PlayClipFade(myBackgroundMusicBass, -1, 0);
    AudioSystem::PlayClipFade(myBackgroundMusicClips[0], 0, 1);
    
    // initialise map
    myTileSize = Game::WindowHeight/8; // 8 tiles up, 9-32 tiles across (avg. 14)
    TilePatterns::Init();
    for (int i = 0; i < 3; i++)
        myUpcomingTiles[i] = TilePatterns::GetPattern(TilePatterns::PATTERNS::LOW_FLOOR_REPEAT);

    myTileAdvanceCounter = 0;
    mySpeed = 0.5f;
    myDuration = 0.f;

    myHatTimerMax = 1 / (3 * mySpeed);
    myHatTimerCurrent = -1.f;
    myHatDirection = 1;

    // initialise pointers to other objects
    myPlayer = new Character();
    myTilePooler = new TilePooler();
}

SceneLevel::~SceneLevel(){
    // world objects
    delete myPlayer;
    myPlayer = nullptr;

    delete myTilePooler;
    myTilePooler = nullptr;

    delete myHat;
    myHat = nullptr;

    // ui
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

    // stop bgm, return mix channels 0 and 1 to default sfx volume
    Mix_FadeOutChannel(0, 100);
    Mix_FadeOutChannel(1, 100);
    AudioSystem::SetVolumeClips(AudioSystem::GetVolumeSFX(-1),0);
    AudioSystem::SetVolumeClips(AudioSystem::GetVolumeSFX(-1),1);
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

    SDL_Rect hatPosition = {Game::WindowWidth - myTileSize, myTileSize,(int)(myTileSize*0.8),(int)(myTileSize*0.8)};
    myHat = new UI_Element("data/hat.png", aRenderer, hatPosition, UI_Element::ASPECT_RATIO::NONE);
    if (myHat == nullptr) return false;
    myHatTargetY = myTileSize * 7;

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
        float distance = std::ceilf(deltaTime * mySpeed * myTileSize);
        myTileAdvanceCounter -= distance;
        if (myTileAdvanceCounter < 0){
            myTileAdvanceCounter += myTileSize;
            UpdateUpcomingTiles();
            AdvanceTiles(distance - (myTileSize - myTileAdvanceCounter));
        }

        myTilePooler->Update(deltaTime, mySpeed);
        myPlayer->Update(deltaTime, mySpeed);

        UpdateHatPosition(deltaTime);

        // start speeding up after tutorial
        if (myControlInfoIndex > 2 || myControlInfoIndex < 0)
            mySpeed += (deltaTime * 0.01f);
            
        myFinished = myPlayer->isDead(); // todo start delay to play ghost animation
    }

    // bgm keeps going; todo drop volume when paused
    UpdateBackgroundMusic(); 

    return true;
}

void SceneLevel::UpdateBackgroundColour(float deltaTime){
    // advance hue; swap direction at 100 and 290
    myBackgroundColourHSV[0] += myBackgroundColourChange * deltaTime;
    
    if ((myBackgroundColourHSV[0] < 100 && myBackgroundColourChange == -1)
        || myBackgroundColourHSV[0] > 290 && myBackgroundColourChange == 1){
        // change direction
        if (myBackgroundColourHSV[0] < 100)
            myBackgroundColourChange = 1;
        else myBackgroundColourChange = -1;

        // randomly change saturation or value
        if (std::rand()%2 == 0){
            // saturation between 50% - 90%
            if (myBackgroundColourHSV[1] < 0.5f)
                myBackgroundColourHSV[1] += 0.05f;
            else if (myBackgroundColourHSV[1] < 0.9f)
                myBackgroundColourHSV[1] -= 0.05f;
        }
        else {
            // value between 60% - 85%
            if (myBackgroundColourHSV[2] < 0.6f)
                myBackgroundColourHSV[2] += 0.05f;
            else if (myBackgroundColourHSV[2] > 0.85f)
                myBackgroundColourHSV[2] -= 0.05f;
        }
    }
    //std::cout << "HSV: " << myBackgroundColourHSV[0] << " " << myBackgroundColourHSV[1] << " " << myBackgroundColourHSV[2] << std::endl;
    BackgroundColourHSVToRGB();
}

void SceneLevel::BackgroundColourHSVToRGB(){
    float chroma = myBackgroundColourHSV[2] * myBackgroundColourHSV[1];
    float hueRGB = myBackgroundColourHSV[0]/60.f;
    float intermediateX = chroma * (1 - std::abs(std::remainderf(hueRGB, 2.f) - 1));
    if (intermediateX < 0) intermediateX *= -1; // temporary fix, todo figure out issue

    // find rgb point with same hue and chroma as hsv point
    if (hueRGB < 2){ // hue between 60 and 120; shouldn't be lower than 100 though
        myBackgroundColourRGB[0] = intermediateX;
        myBackgroundColourRGB[1] = chroma;
        myBackgroundColourRGB[2] = 0;
    }
    else if (hueRGB < 3){ // hue between 120 and 180 
        myBackgroundColourRGB[0] = 0;
        myBackgroundColourRGB[1] = chroma;
        myBackgroundColourRGB[2] = intermediateX;
    }
    else if (hueRGB < 4){ // hue between 180 and 240
        myBackgroundColourRGB[0] = 0;
        myBackgroundColourRGB[1] = intermediateX;
        myBackgroundColourRGB[2] = chroma;
    }
    else { // should be hue < 290, hueRGB (h') < 5
        myBackgroundColourRGB[0] = intermediateX;
        myBackgroundColourRGB[1] = 0;
        myBackgroundColourRGB[2] = chroma;
    }

    // match rgb point's value 
    float valueDifference = myBackgroundColourHSV[2] - chroma;
    myBackgroundColourRGB[0] = std::roundf(255 * (myBackgroundColourRGB[0] + valueDifference));
    myBackgroundColourRGB[1] = std::roundf(255 * (myBackgroundColourRGB[1] + valueDifference));
    myBackgroundColourRGB[2] = std::roundf(255 * (myBackgroundColourRGB[2] + valueDifference));

    /*std::cout << "vars: "<< chroma << " "<<hueRGB<<" "<<intermediateX<<" "<<valueDifference<<std::endl;
    std::cout << "RGB: " << myBackgroundColourRGB[0] << " " << myBackgroundColourRGB[1] << " " << myBackgroundColourRGB[2] << std::endl << std::endl;*/
}

void SceneLevel::UpdateBackgroundMusic(){
    // play new clip if the previous one ended
    if (Mix_Playing(1) == 0){
        // pick different melody clip based on bg colour
        int melodyIndex = -1;
        if (myBackgroundColourHSV[0] < 120)
            melodyIndex = 0;
        else if (myBackgroundColourHSV[0] < 180)
            melodyIndex = 1;
        else if (myBackgroundColourHSV[0] < 240)
            melodyIndex = 2;
        else melodyIndex = 3;

        AudioSystem::PlayClip(myBackgroundMusicClips[melodyIndex], 0, 1);
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

void SceneLevel::AdvanceTiles(int offset){
    for (int i = 0; i < 8; i++){
        // is there a tile here?
        int FlagToCheck = (1 << i);
        if ((*myUpcomingTiles[1] & FlagToCheck) == 0) continue;

        // determine sprite based on adjacent tiles
        int UpperFlagToCheck = (1 << (i-1));
        int LowerFlagToCheck = (1 << (i+1));

        int index = 0;
        if ((*myUpcomingTiles[0] & FlagToCheck) == 0) // left empty
            index += 1;
        if ((*myUpcomingTiles[2] & FlagToCheck) == 0) // right empty
            index += 2;
        if (i == 0 || ((*myUpcomingTiles[1] & UpperFlagToCheck) == 0)) // up
            index += 4;
        if (i == 7 || ((*myUpcomingTiles[1] & LowerFlagToCheck) == 0)) // down
            index += 8;

        SDL_Point spritePosition = { 300 * (index % 4), 300 * ((index - (index % 4)) / 4)};
        // put new tile at far right of the screen
        int tileX = (myTileMaxX * myTileSize) + offset;
        myTilePooler->SetFreeTile({tileX, i*myTileSize},spritePosition);
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
    // find the highest tile in the same column as the hat
    SDL_Rect hatColumn = myHat->GetDimensions();
    hatColumn.h = Game::WindowHeight;
    hatColumn.y = hatColumn.h/2;
    hatColumn.x += hatColumn.w * 1.5f;
    
    std::vector<Tile*> tiles = myTilePooler->GetTilesCollidingWith(hatColumn);
    // keep original target if column is empty
    if (!tiles.empty()){
        int highestY = myTileSize * 8;
        for (int i = 0; i < tiles.size(); i++){
            if (tiles[i]->GetPosition().y < highestY){
                highestY = tiles[i]->GetPosition().y;
            }
        }
        myHatTargetY = highestY - myTileSize;
    }

    // move to target or bob around it if already there
    SDL_Point hatPosition = {myHat->GetDimensions().x, myHat->GetDimensions().y};

    if (hatPosition.y == myHatTargetY){
        myHatTimerMax = 1 / (3 * mySpeed);
        if (myHatTimerMax < 0.05f) myHatTimerMax = 0.05f;

        myHatDirection *= -1;
        myHatTimerCurrent = myHatTimerMax;
    }
    else if (myHatTimerCurrent < 0) {
        myHatTargetY > hatPosition.y ? myHatDirection = 1 : myHatDirection = -1;
    }
    else myHatTimerCurrent -= deltaTime;

    hatPosition.y += std::ceilf(myTileSize * 0.5f * deltaTime) * myHatDirection;
    myHat->SetPosition(hatPosition);
}

void SceneLevel::Render(SDL_Renderer* aRenderer){
    // background changes colour
    SDL_SetRenderDrawColor(aRenderer, myBackgroundColourRGB[0], myBackgroundColourRGB[1], myBackgroundColourRGB[2], SDL_ALPHA_OPAQUE);
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