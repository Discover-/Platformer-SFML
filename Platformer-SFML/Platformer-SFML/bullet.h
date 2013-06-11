#pragma once

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
#include "game.h"
#include "position.h"

class Bullet : public Position
{
    public:
        Bullet(Game* _game, sf::RenderWindow* _window, float _x, float _y, sf::Texture _imageBullet, bool _movingToLeft, float _velocity = 5);
        ~Bullet();

        void Update();
        bool IsRemoved() { return isRemoved; }

        void SetVelocity(float val) { velocity = val; }
        float GetVelocity() { return velocity; }
        void Explode();

        void Draw(sf::Sprite* _spriteBullet = NULL, bool updatePos = false);

    private:
        Game* game;
        bool isRemoved;
        float velocity;
        bool movingToLeft;
        sf::RenderWindow* window;
        sf::Texture imageBullet;
};
