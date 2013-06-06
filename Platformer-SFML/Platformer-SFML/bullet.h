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

class Game;

class Bullet
{
    public:
        Bullet(Game* _game, sf::RenderWindow* _window, float _x, float _y, sf::Texture _imageBullet, bool _movingToLeft, float _velocity = 5);
        ~Bullet();

        void Update();
        bool IsRemoved() { return isRemoved; }
        float GetPosX() { return posX; }
        float GetPosY() { return posY; }
        sf::Vector2f GetPosXY() { return sf::Vector2f(posX, posY); }
        void SetPosX(float val);
        void SetPosY(float val);
        void SetPosXY(float valX, float valY);
        void SetVelocity(float val) { velocity = val; }
        float GetVelocity() { return velocity; }
        void Explode();

        //sf::Sprite GetSpriteBullet() { return spriteBullet; }
        //void SetSprite(sf::Sprite sprite) { spriteBullet = sprite; }

        void Draw(sf::Sprite* _spriteBullet = NULL, bool updatePos = false);

    private:
        Game* game;
        bool isRemoved;
        float posX, posY;
        float velocity;
        bool movingToLeft;
        sf::RenderWindow* window;
        sf::Texture imageBullet;
};
