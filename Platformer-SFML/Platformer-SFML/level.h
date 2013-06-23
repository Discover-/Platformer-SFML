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
        Level(Game* _game);
        ~Level();

        void LoadAllImages();
        void LoadMap(std::string filename, sf::RenderWindow &window, bool reload = false);
        void DrawMap(sf::RenderWindow &window, bool menuLevel = false);
        int GetAmountOfTiles(std::string filename);//std::vector<std::vector<std::string>> &tilesInfoLayers, std::vector<std::string> &tilesInfoBlocks);

        int GetCurrentLevel() { return currLevel; }

        static std::map<std::string /* filename */, sf::Texture> Textures;

    private:
        Game* game;
        std::vector<SpriteInfo> sprites;
        int currLevel;
};
