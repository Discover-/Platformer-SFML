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

Player::Player(Game* _game, sf::RenderWindow _window, float x, float y)
{
    window = _window;
    posX = x;
    posY = y;
    game = _game;

    for (int i = 0; i < 4; ++i)
        keysDown[i] = false;

    isJumping = false;
    isFalling = false;
    fallSpeed = 0;
    jumpSpeed = 150;
}

void Player::Update()
{
    if (isJumping)
    {
        if (jumpSpeed)
        {
            spriteCharacter.setPosition(posX, posY - jumpSpeed);
            jumpSpeed--;
        }
        else
        {
            isJumping = false;
            //isFalling = true;
            jumpSpeed = 15;
        }
    }

    if (isFalling)
    {
        spriteCharacter.setPosition(pos.x, pos.y + fallSpeed);
        fallSpeed++;

        if (!fallSpeed)
        {
            isFalling = false;
            fallSpeed = 0;
        }
    }

    if (posX < 0)
        posX = 0;

    if (posY < 0)
        posY = 0;

    if (posX > 938)
        posX = 938;

    if (posY > 544)
        posY = 544;
}

void Player::Draw(sf::Sprite* _spriteCharacter /* = NULL */)
{
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
            case 0: //! ^ & W
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
