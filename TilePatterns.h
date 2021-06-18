#ifndef TILEPATTERNS_H
#define TILEPATTERNS_H

#include <map>
#include <vector>

class TilePatterns{
public:
    enum class PATTERNS{
        GAP = 0,
        LOW_FLOOR,
        LOW_JUMP,
        LOW_SLIDE,
        HIGH_FLOOR,
        HIGH_JUMP,
        HIGH_SLIDE
    };
    static void Init();
    static std::vector<unsigned char> GetPattern(PATTERNS key);
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
    static std::map<int, std::vector<unsigned char>> myPatterns;
};

#endif // TILEPATTERNS_H