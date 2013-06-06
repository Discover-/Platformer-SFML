#pragma once

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

inline std::string GetTileFilename(int tileId, bool &isCollidable)
{
    isCollidable = false;

    switch (tileId)
    {
        case 0: //! cloud_*.png
            if (urand(0, 20) < 18)
                break;

            return "Graphics/Tiles/cloud_" + std::to_string(long double(urand(0, 2))) + ".png";
        case 1: //! block.png
            isCollidable = true;
            return "Graphics/Tiles/block.png";
        case 2: //! ground.png
            isCollidable = true;
            return "Graphics/Tiles/ground.png";
        case 3: //! ground_dirt.png
            isCollidable = true;
            return "Graphics/Tiles/ground_dirt.png";
        case 4: //! bridge.png
            isCollidable = true;
            return "Graphics/Tiles/bridge.png";
        //! DECOR!
        case 5: //! grass.png
            return "Graphics/Tiles/grass.png";
        case 6: //! hill_short.png
            return "Graphics/Tiles/hill_short.png";
        case 7: //! hill_long.png
            return "Graphics/Tiles/hill_long.png";
        case 8: //! fence.png
            return "Graphics/Tiles/fence.png";
        default:
            break;
    }

    return "";
}


