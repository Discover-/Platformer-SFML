#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

enum GameState
{
    STATE_MAIN_MENU     = 0,
    STATE_PAUSED        = 1,
    STATE_PAUSED_FOCUS  = 2,
    STATE_PLAYING       = 3,
};

#define GAME_STATE_DRAW_GAME(x) (x == STATE_PAUSED || x == STATE_PAUSED_FOCUS || x == STATE_PLAYING)
#define GAME_STATE_PAUSED(x) (x == STATE_PAUSED || x == STATE_PAUSED_FOCUS)

enum UnitTypeId
{
    TYPEID_ENEMY        = 0,
    TYPEID_PLAYER       = 1,
    TYPEID_MENU_PLAYER  = 2,
};

enum TileTypeId
{
    TYPEID_MOVING_TILE      = 0,
    TYPEID_BOUNCE_TILE      = 1,
    TYPEID_BONUS_TILE       = 2,
    TYPEID_WATER_TILE       = 3,
    TYPEID_LAVA_TILE        = 4,
    TYPEID_QUICK_SAND_TILE  = 5,
};

struct SpriteInfo
{
    sf::Texture image;
    float posX, posY;
};

enum MenuId
{
    MENU_NONE   = -1,
    MENU_MAIN   = 0,
    MENU_LEVELS = 1,
};

struct MenuInfo
{
    MenuId id;
    bool draw;
    int amountOfButtons;
};

struct MenuButtonInfo
{
    int id;
    sf::RectangleShape rectShape;
    sf::Text textShape;
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

inline bool HitLeftSide(float x1, float y1, float h1, float w1, float x2, float y2, float h2, float w2)
{
    return x1 + w1 <= x2;
}

inline bool HitRightSide(float x1, float y1, float h1, float w1, float x2, float y2, float h2, float w2)
{
    return x2 + w2 <= x1;
}

inline bool HitBottomSide(float x1, float y1, float h1, float w1, float x2, float y2, float h2, float w2)
{
    return y1 + h1 >= y2;
}

inline bool HitTopSide(float x1, float y1, float h1, float w1, float x2, float y2, float h2, float w2)
{
    return y2 + h2 >= y1;
}
