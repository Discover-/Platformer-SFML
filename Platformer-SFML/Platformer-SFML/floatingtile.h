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

class FloatingTile : public Position
{
    public:
        FloatingTile(sf::Vector2f _startPosition);
        ~FloatingTile();

        virtual void Update();

    private:
        bool movingUp;
        sf::Vector2f startPosition;
};
