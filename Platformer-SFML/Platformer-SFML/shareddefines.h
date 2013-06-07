#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

enum GameState
{
    STATE_MENU          = 0,
    STATE_PAUSED        = 1,
    STATE_PAUSED_FOCUS  = 2,
    STATE_PLAYING       = 3,
};

#define GAME_STATE_DRAW_GAME(x) (x == STATE_PAUSED || x == STATE_PAUSED_FOCUS || x == STATE_PLAYING)
#define GAME_STATE_PAUSED_DRAWING(x) (x == STATE_PAUSED || x == STATE_PAUSED_FOCUS)

enum TypeId
{
    TYPEID_ENEMY    = 0,
    TYPEID_PLAYER   = 1,
};

struct SpriteInfo
{
    sf::Texture image;
    float posX, posY;
};

struct MenuSpriteInfo
{
    sf::Texture image;
    float posX, posY;
    int id;
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

inline std::string GetTileFilename(std::string tileString, bool &isCollidable)
{
    isCollidable = false;

    if (tileString == "0")
    {
        if (urand(0, 20) > 18)
            return "Graphics/Tiles/cloud_" + std::to_string(static_cast<long long>(urand(0, 2))) + ".png";
    }
    else if (tileString == "1")
    {
        isCollidable = true;
        return "Graphics/Tiles/block.png";
    }
    else if (tileString == "2")
    {
        isCollidable = true;
        return "Graphics/Tiles/ground.png";
    }
    else if (tileString == "3")
    {
        isCollidable = true;
        return "Graphics/Tiles/ground_dirt.png";
    }
    else if (tileString == "4")
    {
        isCollidable = true;
        return "Graphics/Tiles/bridge.png";
    }
    else if (tileString == "5")
        return "Graphics/Tiles/grass.png";
    else if (tileString == "6")
        return "Graphics/Tiles/hill_short.png";
    else if (tileString == "7")
        return "Graphics/Tiles/hill_long.png";
    else if (tileString == "8")
        return "Graphics/Tiles/fence_normal.png";
    else if (tileString == "9")
        return "Graphics/Tiles/fence_broken.png";
    else if (tileString == "x")
        return "Graphics/Tiles/lava.png";

    return "";
}
