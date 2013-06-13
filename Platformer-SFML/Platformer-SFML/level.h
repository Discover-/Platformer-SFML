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

        void LoadMap(std::string filename, sf::RenderWindow &window, bool reload = false);
        void DrawMap(sf::RenderWindow &window, bool menuLevel = false);

        int GetCurrentLevel() { return currLevel; }

    private:
        Game* game;
        std::vector<SpriteInfo> sprites;
        int currLevel;
};


