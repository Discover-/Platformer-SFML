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
#include "game.h"

class Game;

class MenuPlayer : public Unit
{
    public:
        MenuPlayer(Game* _game, sf::RenderWindow* _window, sf::Vector2f position, std::vector<std::pair<int, sf::Texture>> _spritesRight, UnitTypeId _typeId, int _totalMoveFrames, int _frameInterval);
        ~MenuPlayer();

        void Update();
        void HandleTimers(sf::Int32 diff_time);

    private:

};
