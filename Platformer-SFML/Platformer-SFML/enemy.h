#pragma once

#include "unit.h"
#include "game.h"

class Enemy : public Unit
{
    public:
        Enemy(sf::RenderWindow* _window, sf::Vector2f position, std::vector<std::pair<int, sf::Texture>> _spritesLeft, std::vector<std::pair<int, sf::Texture>> _spritesRight, sf::Texture _imageDead, sf::Texture _bulletTexture, int _life = 3, int _totalMoveFrames = 1, int _frameInterval = 80, bool _canFly = true);
        ~Enemy();

        void Update();
        void HandleTimers(sf::Int32 diff_time);
        sf::Sprite GetSpriteBody() { return Unit::GetSpriteBody(); }

    private:
        float destinationX1, destinationY1;
        float destinationX2, destinationY2;
        sf::Texture imageDead;
};
