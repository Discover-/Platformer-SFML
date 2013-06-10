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
#include "collision.h"
#include "shareddefines.h"
#include "bullet.h"
#include "unit.h"

Enemy::Enemy(Game* _game, sf::RenderWindow* _window, sf::Vector2f position1, sf::Vector2f position2, std::vector<std::pair<int, sf::Texture>> _spritesLeft, std::vector<std::pair<int, sf::Texture>> _spritesRight, UnitTypeId _typeId, int _life, int _totalMoveFrames, int _frameInterval, bool _canFly) :
Unit(_game, _window, position1, _spritesLeft, _spritesRight, _typeId, _life, _totalMoveFrames, _frameInterval, _canFly)
{
    destinationX1 = position1.x;
    destinationY1 = position1.y;
    destinationX2 = position2.x;
    destinationY2 = position2.y;
}

void Enemy::Update()
{
    Unit::Update();

    if (GAME_STATE_PAUSED_DRAWING(GetGame()->GetGameState()) || IsDead())
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
    //if (game->IsPaused() || game->IsInMenu())
    //    return;

    Unit::HandleTimers(diff_time);
}
