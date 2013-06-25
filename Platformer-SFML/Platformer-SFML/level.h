#pragma once

#include <vector>
#include <array>
#include <map>
#include <SFML/Graphics.hpp>
#include "shareddefines.h"
#include "game.h"

class Level
{
    public:
        Level();
        ~Level();

        void LoadAllImages();
        void LoadMap(std::string filename, sf::RenderWindow &window, bool reload = false);
        void DrawMap(sf::RenderWindow &window, bool menuLevel = false);
        int GetAmountOfTiles(std::string filename);

        int GetCurrentLevel() { return currLevel; }

        static std::map<std::string /* filename */, sf::Texture> Textures;

    private:
        std::vector<SpriteInfo> sprites;
        int currLevel;
};
