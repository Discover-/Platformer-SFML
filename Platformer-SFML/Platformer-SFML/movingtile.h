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

class MovingTile : public Position
{
    public:
        MovingTile(Game* _game, sf::RenderWindow* _window, sf::Texture _image, int _velocity, sf::Vector2f startPosition, sf::Vector2f _destination);
        ~MovingTile();

        void Update();
        void AddPassenger(Unit* unit);
        void RemovePassenger(Unit* unit);
        bool HasPassenger(Unit* unit);
        void Draw(sf::Sprite spriteTile, bool updatePos = false);
        sf::Sprite GetSprite();

    private:
        Game* game;
        int velocity;
        sf::Texture image;
        bool movingToLeft;
        sf::RenderWindow* window;
        std::list<Unit*> passengers;
        sf::Vector2f startPosition, destination;
};
