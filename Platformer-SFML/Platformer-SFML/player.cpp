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

Player::Player(Game* _game, sf::RenderWindow* _window, float x, float y, sf::Sprite _spriteBody, TypeId _typeId) : Unit(_game, _window, x, y, _spriteBody, _typeId)
{
    for (int i = 0; i < 4; ++i)
        keysDown[i] = false;
}

void Player::Update()
{
    Unit::Update();
}

void Player::HandleTimers(sf::Int32 diff_time)
{
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
