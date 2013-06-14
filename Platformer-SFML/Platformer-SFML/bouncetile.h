#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <time.h>
#include <unordered_map>
#include <set>
#include <string>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "unit.h"
#include "position.h"
#include "tile.h"

class Game;

class BounceTile : public Tile
{
    public:
        BounceTile(Game* _game, sf::RenderWindow* _window, sf::Texture _image, int _velocity, sf::Vector2f startPosition, std::string color);
        ~BounceTile();

        void Update();
        void HandleTimers(sf::Int32 diff_time);
        bool OnCollision(Unit* unit = NULL); //! Return true if we should stop movement of player when colliding

        bool IsUsed() { return isUsed; }
        void SetIsUsed(bool val) { isUsed = val; }

    private:
        bool isUsed;
        sf::Texture imageUsed;
        sf::Vector2f startPosition;
};
