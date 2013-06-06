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

Enemy::Enemy(Game* _game, sf::RenderWindow* _window, float x1, float y1, float x2, float y2, sf::Sprite _spriteBody, TypeId _typeId) : Unit(_game, _window, x1, y1, _spriteBody, _typeId)
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
        movingForward = !movingForward;

    if (movingForward)
    {
        SetPositionX(GetPositionX() + GetMoveSpeed());

        if (GetPositionX() > destinationX2)
            movingForward = false;
    }
    else
    {
        SetPositionX(GetPositionX() - GetMoveSpeed());

        if (GetPositionX() < destinationX1)
            movingForward = true;
    }
}

void Enemy::HandleTimers(sf::Int32 diff_time)
{
    Unit::HandleTimers(diff_time);
}
