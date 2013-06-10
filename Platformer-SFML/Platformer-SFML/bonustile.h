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

class BonusTile : public Tile
{
    public:
        BonusTile(Game* _game, sf::RenderWindow* _window, sf::Texture _image, sf::Vector2f startPosition);
        ~BonusTile();

        void Update();
        void HandleTimers(sf::Int32 diff_time);

        bool IsUsed() { return isUsed; }
        void SetIsUsed(bool val);

    private:
        bool isUsed, animating, animationFinished, movingUp;
        sf::Texture imageUsed;
        sf::Vector2f startPosition;
};
