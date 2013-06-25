#pragma once

#include <vector>
#include <array>
#include <SFML/Graphics.hpp>
#include "shareddefines.h"
#include "game.h"

class Menu
{
    public:
        Menu();
        ~Menu();

        void Load();
        void Update(sf::RenderWindow &window);
        void Draw(sf::RenderWindow &window);

        int GetSelectedOption() { return selectedOption; }
        void SetSelectedOption(int val) { selectedOption = val; }
        void UpdateSelection(bool moveNegative);

        void PressedEnterOrMouse(sf::RenderWindow &window);

        MenuId GetCurrentMenu() { return currentMenu; }
        void SetCurrentMenu(MenuId newMenu) { currentMenu = newMenu; }
        MenuInfo GetCurrentMenuInfo() { return menus[currentMenu].first; }

    private:
        sf::Font font;
        std::vector<std::pair<MenuInfo /* menuId, bool draw */, std::vector<MenuButtonInfo> /* menuInfo */>> menus;
        int selectedOption;
        sf::Vector2i prevMousePos;
        bool movingCurrMenuOut, movingNewMenuIn;
        MenuId currentMenu, newMenu;
};
