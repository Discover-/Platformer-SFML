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

class Player : public Unit
{
    public:
        Player(sf::RenderWindow* _window, sf::Vector2f position, std::vector<std::pair<int, sf::Texture>> _spritesLeft, std::vector<std::pair<int, sf::Texture>> _spritesRight, sf::Texture _imageHeartEmpty, sf::Texture _imageHeartFull, sf::Texture _imageSmallCoin, sf::Texture _imageJumpSpriteLeft, sf::Texture _imageJumpSpriteRight, sf::Texture _bulletTexture, int _life = 5, int _totalMoveFrames = 9, int _frameInterval = 30);
        ~Player();

        void Update();
        void HandleTimers(sf::Int32 diff_time);
        sf::Sprite GetSpriteBody();

        void DrawAccessoires(sf::RenderWindow &window, sf::View &view);
        std::vector<std::pair<int /* id */, bool /* full */>> &GetHearts() { return hearts; }

        std::vector<std::pair<int, sf::Texture>> GetSpritesLeft() { return spritesLeft; }

    private:
        int coinAmount;
        std::vector<std::pair<int /* id */, bool /* full */>> hearts;
        sf::Texture imageHeartEmpty, imageHeartFull, imageSmallCoin;
        sf::Texture imageJumpSpriteLeft, imageJumpSpriteRight;
        std::vector<std::pair<int, sf::Texture>> spritesLeft;
};
