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
#include "player.h"
#include "collision.h"
#include "shareddefines.h"
#include "bullet.h"
#include "enemy.h"
#include "game.h"

Player::Player(Game* _game, sf::RenderWindow* _window, float x, float y, std::vector<std::pair<int, sf::Texture>> _spritesLeft, std::vector<std::pair<int, sf::Texture>> _spritesRight, TypeId _typeId, int _life, int _totalMoveFrames, int _frameInterval, bool _canFly) :
Unit(_game, _window, x, y, _spritesLeft, _spritesRight, _typeId, _life, _totalMoveFrames, _frameInterval, _canFly)
{
    for (int i = 0; i < 4; ++i)
        keysDown[i] = false;

    imageHeartEmpty.loadFromFile("Graphics/Other/heart_empty.png");
    imageHeartFull.loadFromFile("Graphics/Other/heart_full.png");

    for (int i = 0; i < 5; ++i)
        hearts.push_back(std::make_pair(i, true));
}

void Player::Update()
{
    Unit::Update();

    if (GetGame()->GetGameState() != STATE_PLAYING)
        return;

    SetIsMoving(false);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if (!CollidesWithGameobjects(GetPositionX() - GetMoveSpeed(), GetPositionY()))
        {
            SetPosition(GetPositionX() - GetMoveSpeed(), GetPositionY());
            SetIsMovingToLeft(false);
            SetIsMoving(true);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if (!CollidesWithGameobjects(GetPositionX() + GetMoveSpeed(), GetPositionY()))
        {
            SetPosition(GetPositionX() + GetMoveSpeed(), GetPositionY());
            SetIsMovingToLeft(true);
            SetIsMoving(true);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        if (!IsJumping() && /*!IsBouncing() &&*/ !IsFalling())
            SetIsJumping(true);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        if (CanShoot())
            Shoot();

    std::vector<Enemy*> enemies = GetGame()->GetEnemies();

    for (std::vector<Enemy*>::iterator itr = enemies.begin(); itr != enemies.end(); ++itr)
    {
        if ((*itr)->IsDead())
            continue;

        float enemyX, enemyY;
        (*itr)->GetPosition(enemyX, enemyY);

        if (IsInRange(GetPositionX(), enemyX, GetPositionY(), enemyY, 150.0f))
        {
            sf::FloatRect boundsEnemy = (*itr)->GetSpriteBody().getGlobalBounds();
            sf::FloatRect boundsPlayer = GetSpriteBody().getGlobalBounds();

            if (WillCollision(GetPositionX(), GetPositionY(), boundsPlayer.height, boundsPlayer.width, enemyX, enemyY, boundsEnemy.height, boundsEnemy.width))
            {
                BounceAway(!(*itr)->IsMovingToLeft());
                break;
            }
        }
    }
}

void Player::HandleTimers(sf::Int32 diff_time)
{
    if (GetGame()->GetGameState() != STATE_PLAYING)
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

void Player::DrawHearts(sf::RenderWindow &window, sf::View &view)
{
    for (std::vector<std::pair<int /* id */, bool /* full */>>::iterator itr = hearts.begin(); itr != hearts.end(); ++itr)
    {
        sf::Texture imageHeart = (*itr).second ? imageHeartFull : imageHeartEmpty;
        sf::Sprite spriteHeart(imageHeart);
        spriteHeart.setPosition(view.getCenter().x - 420.0f - ((*itr).first * 18.0f), view.getCenter().y - 295.0f);

        if (GAME_STATE_PAUSED_DRAWING(GetGame()->GetGameState()))
            spriteHeart.setColor(sf::Color(255, 255, 255, 128));

        window.draw(spriteHeart);
    }
}
