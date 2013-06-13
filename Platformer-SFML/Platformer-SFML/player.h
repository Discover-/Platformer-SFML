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

class Player : public Unit
{
    public:
        Player(Game* _game, sf::RenderWindow* _window, sf::Vector2f position, std::vector<std::pair<int, sf::Texture>> _spritesLeft, std::vector<std::pair<int, sf::Texture>> _spritesRight, int _life = 5, int _totalMoveFrames = 9, int _frameInterval = 30, bool _canFly = false);
        ~Player();

        void Update();
        void SetKeysDown(sf::Uint8 index, bool value);
        void HandleTimers(sf::Int32 diff_time);
        void DrawAccessoires(sf::RenderWindow &window, sf::View &view);
        std::vector<std::pair<int /* id */, bool /* full */>> &GetHearts() { return hearts; }

        std::vector<std::pair<int, sf::Texture>> GetSpritesLeft() { return spritesLeft; }

    private:
        bool keysDown[4];
        int coinAmount;
        std::vector<std::pair<int /* id */, bool /* full */>> hearts;
        sf::Texture imageHeartEmpty;
        sf::Texture imageHeartFull;
        sf::Texture imageSmallCoin;
        std::vector<std::pair<int, sf::Texture>> spritesLeft;
};
