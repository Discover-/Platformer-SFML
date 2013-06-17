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
#include "game.h"
#include "enemy.h"
#include "shareddefines.h"
#include "bullet.h"
#include "unit.h"

Enemy::Enemy(Game* _game, sf::RenderWindow* _window, sf::Vector2f position, std::vector<std::pair<int, sf::Texture>> _spritesLeft, std::vector<std::pair<int, sf::Texture>> _spritesRight, sf::Texture _imageDead, sf::Texture _bulletTexture, int _life, int _totalMoveFrames, int _frameInterval, bool _canFly) :
Unit(_game, _window, position, _spritesLeft, _spritesRight, TYPEID_ENEMY, _life, _totalMoveFrames, _frameInterval, _canFly, _bulletTexture)
{
    SetPosition(position.x, position.y);
    destinationX1 = position.x;
    destinationY1 = position.y;
    destinationX2 = position.x + 200.0f;
    destinationY2 = position.y;
    imageDead = _imageDead;

    if (_canFly)
        destinationX2 += float(urand(100, 300));
}

Enemy::~Enemy()
{

}

void Enemy::Update()
{
    Unit::Update();

    if (IsDead())
    {
        if (CanFly() && GetPositionY() < 900.0f)
        {
            SetPositionY(GetPositionY() + fallSpeed);
            fallSpeed++;
        }

        sf::Sprite spriteDead(imageDead);
        Draw(&spriteDead, true);
        return;
    }

    if (GAME_STATE_PAUSED(GetGame()->GetGameState()) || IsDead())
        return;

    if (CollidesWithGameobjects(GetPositionX() + GetMoveSpeed(), GetPositionY()))
        return;
        //movingToLeft = !movingToLeft;

    if (IsMovingToLeft())
    {
        SetPosition(GetPositionX() + GetMoveSpeed(), GetPositionY());

        if (GetPositionX() > destinationX2)
            SetIsMovingToLeft(false);
    }
    else
    {
        SetPosition(GetPositionX() - GetMoveSpeed(), GetPositionY());

        if (GetPositionX() < destinationX1)
            SetIsMovingToLeft(true);
    }
}

void Enemy::HandleTimers(sf::Int32 diff_time)
{
    if (GetGame()->GetGameState() != STATE_PLAYING)
        return;

    Unit::HandleTimers(diff_time);
}
