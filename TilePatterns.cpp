#include "TilePatterns.h"
#include <cmath>

std::map<TilePatterns::PATTERNS, unsigned char> TilePatterns::myPatterns;

unsigned char* TilePatterns::GetPattern(PATTERNS key){
    return &myPatterns[key];
}

std::vector<unsigned char*> TilePatterns::GetPattern(PATTERNS key, int size){
    if (size < 1) size = 1;
    int sizeHalf = std::ceilf(size / 2);
    if (sizeHalf < 1) sizeHalf = 1;
    std::vector<unsigned char*> pattern;

    bool upperLevelObstacle = false;
    unsigned char* upperLevelFloor = &myPatterns[PATTERNS::LOW_JUMP_GAP_REPEAT];

    if (key >= PATTERNS::MID_JUMP_BLOCK_START
        /* && key < PATTERNS::HIGH_FLOOR_LOW_FLOOR*/
    ){
        upperLevelObstacle = true;
        upperLevelFloor = &myPatterns[PATTERNS::MID_FLOOR_START];
    }
    /*else if (key >= PATTERNS::HIGH_JUMP_BLOCK_LOW_FLOOR){
        upperLevelObstacle = true;
        upperLevelFloor = myPatterns[PATTERNS::HIGH_FLOOR_LOW_FLOOR_START];
    }*/

    if (upperLevelObstacle) {
        for (int i = 0; i < sizeHalf; i++){
            pattern.push_back(upperLevelFloor);
        }
    }

    for (int i = 0; i < sizeHalf; i++){
        pattern.push_back(&myPatterns[key]);
    }

    PATTERNS nextKey = (PATTERNS)(1+(int)key);
    for (int i = 0; i < size; i++){
        pattern.push_back(&myPatterns[nextKey]);
    }
    
    nextKey = (PATTERNS)(2+(int)key);
    for (int i = 0; i < sizeHalf; i++){
        pattern.push_back(&myPatterns[nextKey]);
    }

    if (upperLevelObstacle) {
        for (int i = 0; i < sizeHalf; i++){
            pattern.push_back(upperLevelFloor);
        }
    }

    return pattern;
}

void TilePatterns::Init(){
    unsigned char gap = 0x00;
    unsigned char low_floor = COLUMN::TILE7;
    unsigned char low_jump = TILE7 | TILE6;
    unsigned char low_slide = COLUMN::TILE7 | COLUMN::TILE5;
    unsigned char low_mid_bridge = TILE6;
    unsigned char mid_floor = COLUMN::TILE5;
    unsigned char mid_jump = COLUMN::TILE5 | COLUMN::TILE4;
    unsigned char mid_slide = COLUMN::TILE5 | COLUMN::TILE3;
    unsigned char mid_high_bridge = COLUMN::TILE4;
    unsigned char high_floor = COLUMN::TILE3;
    unsigned char high_jump = TILE3 | TILE2;
    unsigned char high_slide = TILE3 | TILE1;

    myPatterns[PATTERNS::LOW_FLOOR_START] = low_floor;
    myPatterns[PATTERNS::LOW_FLOOR_REPEAT] = low_floor;
    myPatterns[PATTERNS::LOW_FLOOR_END] = low_floor;

    myPatterns[PATTERNS::LOW_JUMP_BLOCK_SOLID_START] = low_floor;
    myPatterns[PATTERNS::LOW_JUMP_BLOCK_SOLID_REPEAT] = low_jump;
    myPatterns[PATTERNS::LOW_JUMP_BLOCK_SOLID_END] = low_floor;

    myPatterns[PATTERNS::LOW_JUMP_BLOCK_HOLLOW_START] = low_floor;
    myPatterns[PATTERNS::LOW_JUMP_BLOCK_HOLLOW_REPEAT] = low_mid_bridge;
    myPatterns[PATTERNS::LOW_JUMP_BLOCK_HOLLOW_END] = low_floor;

    myPatterns[PATTERNS::LOW_JUMP_GAP_START] = low_floor;
    myPatterns[PATTERNS::LOW_JUMP_GAP_REPEAT] = gap;
    myPatterns[PATTERNS::LOW_JUMP_GAP_END] = low_floor;

    myPatterns[PATTERNS::LOW_SLIDE_START] = low_floor;
    myPatterns[PATTERNS::LOW_SLIDE_REPEAT] = low_slide;
    myPatterns[PATTERNS::LOW_SLIDE_END] = low_floor;

    myPatterns[PATTERNS::MID_FLOOR_START] = low_mid_bridge;
    myPatterns[PATTERNS::MID_FLOOR_REPEAT] = mid_floor;
    myPatterns[PATTERNS::MID_FLOOR_END] = low_mid_bridge;

    myPatterns[PATTERNS::MID_JUMP_BLOCK_START] = mid_floor;
    myPatterns[PATTERNS::MID_JUMP_BLOCK_REPEAT] = mid_jump;
    myPatterns[PATTERNS::MID_JUMP_BLOCK_END] = mid_floor;

    myPatterns[PATTERNS::MID_JUMP_GAP_START] = mid_floor;
    myPatterns[PATTERNS::MID_JUMP_GAP_REPEAT] = gap;
    myPatterns[PATTERNS::MID_JUMP_GAP_END] = mid_floor;

    myPatterns[PATTERNS::MID_SLIDE_START] = mid_floor;
    myPatterns[PATTERNS::MID_SLIDE_REPEAT] = mid_slide;
    myPatterns[PATTERNS::MID_SLIDE_END] = mid_floor;
}