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

        int GetSelectedOption() { return selectedOption; }
        void SetSelectedOption(int val) { selectedOption = val; }

        void PressedEnterOrMouse(sf::RenderWindow &window);

        std::vector<MenuSpriteInfo> GetSprites() { return sprites; }
        std::vector<MenuSpriteInfo> GetSpritesLightup() { return spritesLightup; }
        void GetBothSprites(std::vector<MenuSpriteInfo> &_sprites, std::vector<MenuSpriteInfo> &_spritesLightup) { _sprites = sprites; _spritesLightup = spritesLightup; }

    private:
        Game* game;
        std::vector<MenuSpriteInfo> sprites;
        std::vector<MenuSpriteInfo> spritesLightup;
        int selectedOption;
};


