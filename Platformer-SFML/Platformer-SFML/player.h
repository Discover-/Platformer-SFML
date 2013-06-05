#ifndef __PLAYER_H
#define __PLAYER_H

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
#include "unit.h"

class Game;

class Player : public Unit
{
    public:
        Player(Game* _game, sf::RenderWindow* _window, float x, float y, sf::Sprite _spriteBody, TypeId _typeId);
        ~Player();

        void Update();
        void SetKeysDown(sf::Uint8 index, bool value);
        void HandleTimers(sf::Int32 diff_time);

    private:
        Game* game;
        bool keysDown[4];
};

#endif
