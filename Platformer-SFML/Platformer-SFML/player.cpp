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
#include "player.h"
#include "collision.h"
#include "shareddefines.h"
#include "bullet.h"

Player::Player(Game* _game, sf::RenderWindow* _window, float x, float y, std::vector<std::pair<int, sf::Texture>> _spriteCharacters, TypeId _typeId, int _life, int _totalMoveFrames, int _frameInterval, bool _canFly) :
Unit(_game, _window, x, y, _spriteCharacters, _typeId, _life, _totalMoveFrames, _frameInterval, _canFly)
{
    for (int i = 0; i < 4; ++i)
        keysDown[i] = false;
}

void Player::Update()
{
    Unit::Update();

    if (game->GetGameState() != STATE_PLAYING)
        return;

    SetIsMoving(false);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if (!CollidesWithGameobjects(GetPositionX() - GetMoveSpeed(), GetPositionY()))
        {
            SetPosition(GetPositionX() - GetMoveSpeed(), GetPositionY());
            SetIsMoving(true);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if (!CollidesWithGameobjects(GetPositionX() + GetMoveSpeed(), GetPositionY()))
        {
            SetPosition(GetPositionX() + GetMoveSpeed(), GetPositionY());
            SetIsMoving(true);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        if (!IsJumping() && /*!IsBouncing() &&*/ !IsFalling())
            SetIsJumping(true);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        if (CanShoot())
            Shoot();

    //std::vector<Enemy*> enemies = game->GetEnemies();

    //for (std::vector<Enemy*>::iterator itr = enemies.begin(); itr != enemies.end(); ++itr)
    //{
    //    if ((*itr)->IsDead())
    //        continue;

    //    float enemyX, enemyY;
    //    (*itr)->GetPosition(enemyX, enemyY);
    //    sf::FloatRect boundsEnemy = (*itr)->GetSpriteBody().getGlobalBounds();
    //    sf::Vector2f posEnemy = (*itr)->GetSpriteBody().getPosition();

    //    if (IsInRange(GetPositionX(), enemyX, GetPositionY(), enemyY, 150.0f))
    //    {
    //        if (WillCollision(posEnemy.x, posEnemy.y, boundsEnemy.height, boundsEnemy.width, enemyX, enemyY, boundsEnemy.height, boundsEnemy.width))
    //        {
    //            BounceAway(false);
    //            break;
    //        }
    //    }
    //}
}

void Player::HandleTimers(sf::Int32 diff_time)
{
    if (game->GetGameState() != STATE_PLAYING)
        return;

    Unit::HandleTimers(diff_time);
}

void Player::SetKeysDown(sf::Uint8 index, bool value)
{
    return;
    //keysDown[index] = value;

    //if (value)
    //{
    //    switch (index)
    //    {
    //        case 0: //! A & >
    //            keysDown[2] = false;
    //            break;
    //        case 2: //! v & A
    //            keysDown[0] = false;
    //            break;
    //        case 1: //! < & A
    //            keysDown[3] = false;
    //            break;
    //        case 3: //! > & D
    //            keysDown[1] = false;
    //            break;
    //        default:
    //            break;
    //    }
    //}
}
