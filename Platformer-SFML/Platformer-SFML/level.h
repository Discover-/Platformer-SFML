#pragma once

#include <vector>
#include <array>
#include <SFML/Graphics.hpp>
#include "shareddefines.h"
#include "game.h"

class Level
{
    public:
        Level(Game* _game, sf::RenderWindow &window);
        ~Level();

        void LoadMap(char const* filename, sf::RenderWindow &window);
        void DrawMap(sf::RenderWindow &window, bool menuLevel = false);

    private:
        Game* game;
        std::vector<SpriteInfo> sprites;
};


