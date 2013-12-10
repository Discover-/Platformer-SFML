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
#include <math.h>
#include "player.h"
#include "shareddefines.h"
#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include "coin.h"
//#include "audio.h"

Player::Player(sf::RenderWindow* _window, sf::Vector2f position, std::vector<std::pair<int, sf::Texture>> _spritesLeft, std::vector<std::pair<int, sf::Texture>> _spritesRight, sf::Texture _imageHeartEmpty, sf::Texture _imageHeartFull, sf::Texture _imageSmallCoin, sf::Texture _imageJumpSpriteLeft, sf::Texture _imageJumpSpriteRight, sf::Texture _bulletTexture, int _life, int _totalMoveFrames, int _frameInterval) :
Unit(_window, position, _spritesLeft, _spritesRight, TYPEID_PLAYER, _life, _totalMoveFrames, _frameInterval, false, _bulletTexture)
{
    SetPosition(position.x, position.y);
    spritesLeft = _spritesLeft;
    imageHeartEmpty = _imageHeartEmpty;
    imageHeartFull = _imageHeartFull;
    imageSmallCoin = _imageSmallCoin;
    imageJumpSpriteLeft = _imageJumpSpriteLeft;
    imageJumpSpriteRight = _imageJumpSpriteRight;
    coinAmount = 0;

    for (int i = 0; i < 5; ++i)
        hearts.push_back(std::make_pair(i, true));
}

Player::~Player()
{

}

void Player::Update()
{
    Unit::Update();

    if (GAME_STATE_PAUSED(sGame.GetGameState()))
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
        if (!IsJumping() && !IsBouncing() && !IsFalling())
            Jump();

    std::vector<Enemy*> enemies = sGame.GetEnemies();

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

    std::vector<Coin*> coins = sGame.GetCoins();

    for (std::vector<Coin*>::iterator itr = coins.begin(); itr != coins.end(); ++itr)
    {
        if ((*itr)->IsTaken())
            continue;

        if (IsInRange(GetPositionX(), (*itr)->GetPositionX(), GetPositionY(), (*itr)->GetPositionY(), 150.0f))
        {
            sf::FloatRect boundsCoin = (*itr)->GetSprite().getGlobalBounds();
            sf::FloatRect boundsPlayer = GetSpriteBody().getGlobalBounds();

            if (WillCollision(GetPositionX(), GetPositionY(), boundsPlayer.height, boundsPlayer.width, (*itr)->GetPositionX(), (*itr)->GetPositionY(), boundsCoin.height, boundsCoin.width))
            {
                //sGame.GetAudio()->Play("Audio/picked-up-coin.wav");
                (*itr)->SetIsTaken(true);
                coinAmount++;
                break;
            }
        }
    }
}

void Player::HandleTimers(sf::Int32 diff_time)
{
    if (sGame.GetGameState() != STATE_PLAYING)
        return;

    Unit::HandleTimers(diff_time);
}

void Player::DrawAccessoires(sf::RenderWindow &window, sf::View &view)
{
    for (std::vector<std::pair<int /* id */, bool /* full */>>::iterator itr = hearts.begin(); itr != hearts.end(); ++itr)
    {
        sf::Texture imageHeart = (*itr).second ? imageHeartFull : imageHeartEmpty;
        sf::Sprite spriteHeart(imageHeart);
        spriteHeart.setPosition(view.getCenter().x - 420.0f - ((*itr).first * 18.0f), view.getCenter().y - 295.0f);

        if (GAME_STATE_PAUSED(sGame.GetGameState()))
            spriteHeart.setColor(sf::Color(255, 255, 255, 128));

        window.draw(spriteHeart);
    }
    
    if (coinAmount > 0)
    {
        sf::Sprite spriteSmallCoin(imageSmallCoin);

        for (int i = 0; i < coinAmount; ++i)
        {
            spriteSmallCoin.setPosition(view.getCenter().x - 492.0f + (i * 18.0f), view.getCenter().y - 272.0f);

            if (GAME_STATE_PAUSED(sGame.GetGameState()))
                spriteSmallCoin.setColor(sf::Color(255, 255, 255, 128));

            window.draw(spriteSmallCoin);
        }
    }
}

sf::Sprite Player::GetSpriteBody()
{
    if (IsJumping() || IsFalling())
    {
        sf::Sprite sprite(IsMovingToLeft() ? imageJumpSpriteRight : imageJumpSpriteLeft);
        sprite.setPosition(GetPositionX(), GetPositionY());
        return sprite;
    }

    return Unit::GetSpriteBody();
}
