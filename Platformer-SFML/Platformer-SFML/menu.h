#pragma once

#include <vector>
#include <array>
#include <SFML/Graphics.hpp>
#include "shareddefines.h"
#include "game.h"

class Menu
{
    public:
        Menu(Game* _game);
        ~Menu();

        void Load();
        void Update(sf::RenderWindow &window);
        void Draw(sf::RenderWindow &window);

    private:
        Game* game;
        std::vector<MenuSpriteInfo> sprites;
        std::vector<MenuSpriteInfo> spritesLightup;
        int selectedOption;
};


