#ifndef __SHAREDDEFINES_H
#define __SHAREDDEFINES_H

enum GameState
{
    STATE_MENU          = 0,
    STATE_PAUSED        = 1,
    STATE_PAUSED_FOCUS  = 2,
    STATE_PLAYING       = 3,
};

#define GAME_STATE_DRAW_GAME(x) (x == STATE_PAUSED || x == STATE_PAUSED_FOCUS || x == STATE_PLAYING)

enum TypeId
{
    TYPEID_ENEMY    = 0,
    TYPEID_PLAYER   = 1,
};

struct TileInfo
{
    sf::Texture image;
    float posX, posY;
};

inline int urand(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

inline bool IsInRange(float currX, float xDest, float currY, float yDest, float distance)
{
    return ((currX < xDest - distance && currX > xDest + distance && currY < yDest - distance && currY > yDest + distance) ||
            (currX > xDest - distance && currX < xDest + distance && currY > yDest - distance && currY < yDest + distance));
}

inline bool WillCollision(float x1, float y1, float h1, float w1, float x2, float y2, float h2, float w2)
{
    return !(y1 >= y2 + h2 || x1 >= x2 + w2 || y1 + h1 <= y2 || x1 + w1 <= x2);
}

inline std::string GetTileFilename(int tileId)
{
    std::string randNumb = std::to_string(long double(urand(0, 1)));

    switch (tileId)
    {
        case 9:
            return "Graphics/Tiles/sky_3.png"; //! Empty sky block
        case 0:
            return "Graphics/Tiles/sky_" + std::to_string(long double(urand(0, 20) < 16 ? 3 : urand(0, 2))) + ".png";
        case 1:
            return "Graphics/Tiles/dirt_" + randNumb + ".png";
        case 2:
            return "Graphics/Tiles/dirt_rock_" + randNumb + ".png";
        case 3:
            return "Graphics/Tiles/grass_" + randNumb + ".png";
        case 4:
            return "Graphics/Tiles/grass_ontop_" + randNumb + ".png";
        case 5:
            return "Graphics/Tiles/ground_" + randNumb + ".png";
        case 6:
            return "Graphics/Tiles/sand_" + randNumb + ".png";
        default:
            break;
    }

    return "";
}

//void Load
#endif
