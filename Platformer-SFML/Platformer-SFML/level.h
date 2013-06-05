#ifndef __LEVEL_H
#define __LEVEL_H

#include <vector>
#include <array>
#include <SFML/Graphics.hpp>
#include "shareddefines.h"

class Game;

class Level
{
    public:
        Level(Game* _game);
        ~Level();

        int Update();
        void LoadMap(char const* filename);
        void DrawMap(sf::RenderWindow &window);

    private:
        Game* game;
        std::vector<TileInfo> sprites;
};

#endif
