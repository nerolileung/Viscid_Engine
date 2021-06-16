#include "SceneLevel.h"
#include <cmath>
#include <initializer_list>

SceneLevel::SceneLevel(){
    myFinished = false;
    myPaused = false;

    myBackgroundColour[0] = 28.f - std::rand()%4;
    myBackgroundColour[1] = 150.f + std::rand()%20;
    myBackgroundColour[2] = 201.f - std::rand()%50;
    myBackgroundColourChange[0] = 5;
    myBackgroundColourChange[1] = 10;
    myBackgroundColourChange[2] = -10;

    myTileSize = Game::WindowHeight/8; // 8 tiles up, 9-32 tiles across (avg. 14)
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 7; j++){
            myUpcomingTiles[i][j] = false;
        }
        myUpcomingTiles[i][7] = true;
    }
    mySpeed = 0.3f;

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
    
    myTileAdvanceCounter = myTileSize - myPlayer->GetFarRight();

    return true;
}

bool SceneLevel::Update(float deltaTime){
    // todo peek event and pause if window is minimised

    if (myPaused){
        // update pause menu ui and nothing else
        // if return to main menu button is pressed, set finished
        // if quit button is pressed, return false
        // if restart button is pressed, set finished and unpause
    }
    else {
        UpdateBackgroundColour(deltaTime);
        
        // generate more map and allocate tiles
        myTileAdvanceCounter -= deltaTime;
        if (myTileAdvanceCounter < 0){
            UpdateUpcomingTiles();
            AdvanceTiles();
            myTileAdvanceCounter += 1.f;
        } 

        myTilePooler->Update(deltaTime, mySpeed);
        myPlayer->Update(deltaTime);
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
    for (int i = 0; i < 8; i++){
        myUpcomingTiles[0][i] = myUpcomingTiles[1][i];
        myUpcomingTiles[1][i] = myUpcomingTiles[2][i];
    }

    if (myUpcomingPiece.empty()){
        // todo more interesting pieces; consider making an enum or separate class if this gets really complicated. maybe bitmasking for extra spiciness? read them from a file?
        int pieceIndex = std::rand()%2;
        switch (pieceIndex){
            case 0: // three floor tiles, nothing else
                myUpcomingPiece.push_back(
                    {false,false,false,false,false,false,false,true});
                myUpcomingPiece.push_back(
                    {false,false,false,false,false,false,false,true});
                myUpcomingPiece.push_back(
                    {false,false,false,false,false,false,false,true});
            break;
            case 1: // a floor tile on either side of a two-tile gap
                myUpcomingPiece.push_back(
                    {false,false,false,false,false,false,false,true});
                myUpcomingPiece.push_back(
                    {false,false,false,false,false,false,false,false});
                myUpcomingPiece.push_back(
                    {false,false,false,false,false,false,false,false});
                myUpcomingPiece.push_back(
                    {false,false,false,false,false,false,false,true});
            break;
            default: // a column with top and bottom tiles;
                myUpcomingPiece.push_back({true,false,false,false,false,false,false,true});
            break;
        }
    }
    for (int i = 0; i < 8; i++)
        myUpcomingTiles[2][i] = myUpcomingPiece[0][i];
    myUpcomingPiece.erase(myUpcomingPiece.begin(),myUpcomingPiece.begin()+1);
}

void SceneLevel::AdvanceTiles(){
    for (int i = 0; i < 8; i++){
        // determine sprite based on adjacent tiles
        int index = 0;
        if (!myUpcomingTiles[0][i]) // left empty
            index += 1;
        if (!myUpcomingTiles[2][i]) // right empty
            index += 2;
        if (i == 0 || !myUpcomingTiles[1][i - 1]) // up
            index += 4;
        if (i == 7 || !myUpcomingTiles[1][i + 1]) // down
            index += 8;

        SDL_Point spritePosition = { i % 4, (i - (i % 4)) / 4};
        myTilePooler->SetFreeTile({myTileMaxX, i*myTileSize},spritePosition);
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
        // render pause menu ui
    }
}

HatQuest::SCENES SceneLevel::GetNextScene(){
    if (myPaused) return HatQuest::SCENES::MAIN_MENU;
    if (myPlayer->isDead()) return HatQuest::SCENES::END;
    else return HatQuest::SCENES::PLAYING;
}