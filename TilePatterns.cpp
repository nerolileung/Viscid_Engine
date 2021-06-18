#include "TilePatterns.h"

std::map<TilePatterns::PATTERNS, unsigned char> TilePatterns::myPatterns;

std::vector<unsigned char> TilePatterns::GetPattern(PATTERNS key, int size){
    std::vector<unsigned char> pattern;
    pattern.push_back(myPatterns[key]);
    PATTERNS nextKey = (PATTERNS)(1+(int)key);
    for (int i = 0; i < size; i++){
        pattern.push_back(myPatterns[nextKey]);
    }
    nextKey = (PATTERNS)(2+(int)key);
    pattern.push_back(myPatterns[nextKey]);
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
}

unsigned char TilePatterns::GetRow(int index){
    switch (index){
        case 0: return COLUMN::TILE0; break;
        case 1: return COLUMN::TILE1; break;
        case 2: return COLUMN::TILE2; break;
        case 3: return COLUMN::TILE3; break;
        case 4: return COLUMN::TILE4; break;
        case 5: return COLUMN::TILE5; break;
        case 6: return COLUMN::TILE6; break;
        case 7: return COLUMN::TILE7; break;
        default: return COLUMN::TILE0; break;
    }
}