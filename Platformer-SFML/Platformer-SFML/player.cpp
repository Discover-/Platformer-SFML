#include <iostream>
#include <vector>
//#include <cmath>
#include <cstring>
#include <map>
#include <time.h>
#include <unordered_map>
#include <set>
#include <string>
//#include <SFML/Graphics.hpp>
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

Player::Player(Game* _game, sf::RenderWindow* _window, float x, float y, sf::Sprite _spriteCharacter)
{
    window = _window;
    posX = x;
    posY = y;
    game = _game;
    spriteCharacter = _spriteCharacter;

    for (int i = 0; i < 4; ++i)
        keysDown[i] = false;

    isJumping = false;
    isFalling = true;
    fallSpeed = 0;
    jumpSpeed = 15;
    moveSpeed = 10.0f;
}

void Player::Update()
{
    if (isJumping)
    {
        if (jumpSpeed && !CollidesWithGameobjects(posX, posY - jumpSpeed))
        {
            SetPosY(posY - jumpSpeed);
            jumpSpeed--;
        }
        else
        {
            isJumping = false;
            isFalling = true;
            jumpSpeed = 15;
        }
    }

    if (isFalling)
    {
        if (!CollidesWithGameobjects(posX, posY + fallSpeed + 5.0f))
        {
            SetPosY(posY + fallSpeed);
            fallSpeed++;
        }
        else
        {
            //SetPosY(posY + 5.0f);
            isFalling = false;
            fallSpeed = 0;
            jumpSpeed = 15;
        }
    }

    if (posX < 0)
        SetPosX(0.0f);

    if (posY < 0)
        SetPosY(0.0f);

    //if (posX > 900)
     //   SetPosX(900.0f);

    //if (posY > 500)
    //    SetPosY(500.0f);

    Draw(NULL, true);
}

void Player::Draw(sf::Sprite* _spriteCharacter /* = NULL */, bool updatePos /* = false */)
{
    sf::Sprite* spriteToDraw = _spriteCharacter ? _spriteCharacter : &spriteCharacter;

    if (updatePos)
        spriteToDraw->setPosition(posX, posY);

    window->draw(*spriteToDraw);
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

void Player::SetPosX(float val)
{
    SetPosXY(val, posY);
}

void Player::SetPosY(float val)
{
    SetPosXY(posX, val);
}

void Player::SetPosXY(float valX, float valY)
{
    posX = valX;
    posY = valY;
    spriteCharacter.setPosition(posX, posY);
}

bool Player::CollidesWithGameobjects(float newPosX /* = 0.0f */, float newPosY /* = 0.0f */)
{
    sf::Sprite charSprite = spriteCharacter;

    if (newPosX != 0.0f && newPosY != 0.0f)
        charSprite.setPosition(newPosX, newPosY);

    std::vector<sf::Sprite> gameObjects = game->GetGameObjectsCollidable();
    for (std::vector<sf::Sprite>::iterator itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
        if (Collision::PixelPerfectTest(charSprite, (*itr)))
            return true;

    return false;
}

void Player::HandleTimers(sf::Int32 diff_time)
{
    if (!canShoot)
    {
        if (diff_time >= shootCooldown)
        {
            shootCooldown = 0;
            canShoot = true;
        }
        else
            shootCooldown -= diff_time;
    }
}

void Player::Shoot()
{
    shootCooldown = 400;
    canShoot = false;
    //Bullet* bullet = new Bullet(game, window, posX + 50, posY + 20);

    sf::Texture imageBullet;
    imageBullet.loadFromFile("tux_frame_0.png");
    sf::Sprite spriteBullet(imageBullet);
    Bullet* bullet = new Bullet(game, window, posX + 150, posY + 20, spriteBullet);
    game->AddBullet(bullet);
}
