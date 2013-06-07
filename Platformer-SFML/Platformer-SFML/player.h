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
#include "Windows.h"
#include "unit.h"
#include "game.h"

class Game;

class Player : public Unit
{
    public:
        Player(Game* _game, sf::RenderWindow* _window, float x, float y, std::vector<std::pair<int, sf::Texture>> _spritesLeft, std::vector<std::pair<int, sf::Texture>> _spritesRight, TypeId _typeId, int _life, int _totalMoveFrames, int _frameInterval, bool _canFly);
        ~Player();

        void Update();
        void SetKeysDown(sf::Uint8 index, bool value);
        void HandleTimers(sf::Int32 diff_time);
        void DrawHearts(sf::RenderWindow &window, sf::View &view);
        std::vector<std::pair<int /* id */, bool /* full */>> &GetHearts() { return hearts; }

    private:
        Game* game;
        bool keysDown[4];
        std::vector<std::pair<int /* id */, bool /* full */>> hearts;
        sf::Texture imageHeartEmpty;
        sf::Texture imageHeartFull;
};
