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
#include "game.h"
#include "enemy.h"
#include "collision.h"
#include "shareddefines.h"
#include "bullet.h"
#include "unit.h"

Enemy::Enemy(Game* _game, sf::RenderWindow* _window, float x1, float y1, float x2, float y2, std::vector<std::pair<int, sf::Texture>> _spriteEnemies, TypeId _typeId, int _life, int _totalMoveFrames, int _frameInterval, bool _canFly) :
Unit(_game, _window, x1, y1, _spriteEnemies, _typeId, _life, _totalMoveFrames, _frameInterval, _canFly)
{
    destinationX1 = x1;
    destinationY1 = y1;
    destinationX2 = x2;
    destinationY2 = y2;
    movingForward = true;
}

void Enemy::Update()
{
    Unit::Update();

    if (CollidesWithGameobjects(GetPositionX() + GetMoveSpeed(), GetPositionY()))
        return;
        //movingForward = !movingForward;

    if (movingForward)
    {
        SetPosition(GetPositionX() + GetMoveSpeed(), GetPositionY());

        if (GetPositionX() > destinationX2)
            movingForward = false;
    }
    else
    {
        SetPosition(GetPositionX() - GetMoveSpeed(), GetPositionY());

        if (GetPositionX() < destinationX1)
            movingForward = true;
    }
}

void Enemy::HandleTimers(sf::Int32 diff_time)
{
    //if (game->IsPaused() || game->IsInMenu())
    //    return;

    Unit::HandleTimers(diff_time);
}
