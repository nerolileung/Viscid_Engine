#include "TilePatterns.h"

std::map<int,unsigned char> TilePatterns::myPatterns;

unsigned char TilePatterns::GetPattern(PATTERNS key){
    return myPatterns[(int)key];
}

void TilePatterns::Init(){
    myPatterns[(int)PATTERNS::GAP] = 0x00;
    myPatterns[(int)PATTERNS::LOW_FLOOR] = COLUMN::TILE7;
    myPatterns[(int)PATTERNS::LOW_JUMP] = COLUMN::TILE7 | COLUMN::TILE6;
    myPatterns[(int)PATTERNS::LOW_SLIDE] = COLUMN::TILE6 | COLUMN::TILE4;
    
    myPatterns[(int)PATTERNS::HIGH_FLOOR] = COLUMN::TILE3;
    myPatterns[(int)PATTERNS::HIGH_JUMP] = COLUMN::TILE3 | COLUMN::TILE2;
    myPatterns[(int)PATTERNS::HIGH_SLIDE] = COLUMN::TILE0 | COLUMN::TILE2;
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