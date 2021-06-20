#ifndef TILEPATTERNS_H
#define TILEPATTERNS_H

#include <map>
#include <vector>

class TilePatterns{
public:
    enum class PATTERNS{
        LOW_FLOOR_START = 0,
        LOW_FLOOR_REPEAT,
        LOW_FLOOR_END,
        LOW_JUMP_BLOCK_SOLID_START,
        LOW_JUMP_BLOCK_SOLID_REPEAT,
        LOW_JUMP_BLOCK_SOLID_END,
        LOW_JUMP_BLOCK_HOLLOW_START,
        LOW_JUMP_BLOCK_HOLLOW_REPEAT,
        LOW_JUMP_BLOCK_HOLLOW_END,
        LOW_JUMP_GAP_START,
        LOW_JUMP_GAP_REPEAT,
        LOW_JUMP_GAP_END,
        LOW_SLIDE_START,
        LOW_SLIDE_REPEAT,
        LOW_SLIDE_END,
        MID_FLOOR_START,
        MID_FLOOR_REPEAT,
        MID_FLOOR_END,
        /*MID_JUMP_BLOCK,
        MID_JUMP_GAP,
        MID_SLIDE,
        HIGH_FLOOR_LOW_FLOOR,
        HIGH_FLOOR_LOW_JUMP_BLOCK,
        HIGH_FLOOR_LOW_JUMP_GAP,
        HIGH_FLOOR_LOW_SLIDE,
        HIGH_JUMP_BLOCK_LOW_FLOOR,
        HIGH_JUMP_BLOCK_LOW_JUMP_BLOCK,
        HIGH_JUMP_BLOCK_LOW_JUMP_GAP,
        HIGH_JUMP_BLOCK_LOW_SLIDE,
        HIGH_JUMP_GAP_LOW_FLOOR,
        HIGH_JUMP_GAP_LOW_JUMP_BLOCK,
        HIGH_JUMP_GAP_LOW_JUMP_GAP,
        HIGH_JUMP_GAP_LOW_SLIDE,
        HIGH_SLIDE_LOW_FLOOR,
        HIGH_SLIDE_LOW_JUMP_BLOCK,
        HIGH_SLIDE_LOW_JUMP_GAP,
        HIGH_SLIDE_LOW_SLIDE,*/
    };
    static void Init();
    static std::vector<unsigned char> GetPattern(PATTERNS key, int size);
    static unsigned char GetPattern(PATTERNS key);
    static int size() { return myPatterns.size(); };
    static unsigned char GetRow(int index);
private:
    enum COLUMN {
        TILE0 = 1 << 0, // 00000001 or 0x01
        TILE1 = 1 << 1,
        TILE2 = 1 << 2,
        TILE3 = 1 << 3,
        TILE4 = 1 << 4,
        TILE5 = 1 << 5,
        TILE6 = 1 << 6,
        TILE7 = 1 << 7 // 10000000 or 0x128
    };
    static std::map<PATTERNS, unsigned char> myPatterns;
};

#endif // TILEPATTERNS_H