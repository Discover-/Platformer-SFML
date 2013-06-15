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

class Game;

class Coin : public Position
{
    public:
        Coin(Game* _game, sf::RenderWindow* _window, sf::Vector2f _startPosition, sf::Texture _image);
        ~Coin();

        void Update();
        void Draw(sf::Sprite spriteCoin, bool updatePos = false);
        sf::Sprite GetSprite();

        bool IsTaken() { return isTaken; }
        void SetIsTaken(bool val) { isTaken = val; }

    private:
        Game* game;
        bool isTaken;
        bool movingUp;
        sf::Texture image;
        sf::RenderWindow* window;
        sf::Vector2f startPosition;
};
