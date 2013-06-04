#include "shareddefines.h"
#include <iostream>
#include <vector>
//#include <cmath>
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
    isFalling = false;
    fallSpeed = 0;
    jumpSpeed = 15;
}

void Player::Update()
{
    if (isJumping)
    {
        if (jumpSpeed)
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
        SetPosY(posY + fallSpeed);
        fallSpeed++;

        if (fallSpeed > 15)
        {
            isFalling = false;
            fallSpeed = 0;
        }
    }

    if (posX < 0)
        SetPosX(0.0f);

    if (posY < 0)
        SetPosY(0.0f);

    if (posX > 900)
        SetPosX(900.0f);

    if (posY > 500)
        SetPosY(500.0f);

    Draw(NULL, true);
}

void Player::Draw(sf::Sprite* _spriteCharacter /* = NULL */, bool updatePos /* = false */)
{
    if (updatePos)
        _spriteCharacter ? _spriteCharacter->setPosition(posX, posY) : spriteCharacter.setPosition(posX, posY);

    window->draw(_spriteCharacter ? *_spriteCharacter : spriteCharacter);
}

void Player::HandleTimers(unsigned int diff_time)
{
    return;
}

void Player::SetKeysDown(sf::Uint8 index, bool value)
{
    keysDown[index] = value;

    if (value)
    {
        switch (index)
        {
            case 0: //! A & >
                keysDown[2] = false;
                break;
            case 2: //! v & A
                keysDown[0] = false;
                break;
            case 1: //! < & A
                keysDown[3] = false;
                break;
            case 3: //! > & D
                keysDown[1] = false;
                break;
            default:
                break;
        }
    }
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
