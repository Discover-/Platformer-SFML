#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <time.h>
#include <unordered_map>
#include <set>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "unit.h"
#include "position.h"
#include "floatingtile.h"

class Coin : public FloatingTile
{
    public:
        Coin(sf::RenderWindow* _window, sf::Vector2f _startPosition, sf::Texture _image);
        ~Coin();

        void Update();

        void Draw(sf::Sprite spriteCoin, bool updatePos = false);
        sf::Sprite GetSprite();

        bool IsTaken() { return isTaken; }
        void SetIsTaken(bool val) { isTaken = val; }

    private:
        bool isTaken;
        sf::Texture image;
        sf::RenderWindow* window;
};
