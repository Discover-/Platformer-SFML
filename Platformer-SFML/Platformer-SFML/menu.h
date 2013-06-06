#ifndef __MENU_H
#define __MENU_H

#include <vector>
#include <array>
#include <SFML/Graphics.hpp>
#include "shareddefines.h"

class Game;

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

#endif
