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

class Game;

class Player
{
    public:
        Player(Game* _game, sf::RenderWindow* _window, float x, float y, sf::Sprite _spriteCharacter);
        ~Player();

        void Update();
        void SetKeysDown(sf::Uint8 index, bool value);
        void HandleTimers(sf::Int32 diff_time);
        void Draw(sf::Sprite* _spriteCharacter = NULL, bool updatePos = false);
        void SetSpriteBody(sf::Sprite sprite) { spriteCharacter = sprite; }
        sf::Sprite GetSpriteBody() { return spriteCharacter; }

        /* MOVEMENT */
        float GetPosX() { return posX; }
        float GetPosY() { return posY; }
        sf::Vector2f GetPosXY() { return sf::Vector2f(posX, posY); }
        void SetPosX(float val);
        void SetPosY(float val);
        void SetPosXY(float valX, float valY);

        void SetIsJumping(bool val) { isJumping = val; }
        void SetIsFalling(bool val) { isFalling = val; }
        bool IsJumping() { return isJumping; }
        bool IsFalling() { return isFalling; }

        bool CollidesWithGameobjects(float newPosX = 0.0f, float newPosY = 0.0f);

        float GetMoveSpeed() { return moveSpeed; }
        void SetMoveSpeed(float val) { moveSpeed = val; }

        /* MECHANICS */
        void Shoot();
        bool CanShoot() { return canShoot; }
        void SetCanShoot(bool val) { canShoot = val; }
        void SetShootCooldown(sf::Int32 val) { shootCooldown = val; }

    private:
        Game* game;
        float posX, posY;
        bool keysDown[4];
        sf::Sprite spriteCharacter;
        sf::RenderWindow* window;

        /* MOVEMENT */
        bool isJumping, isFalling;
        int fallSpeed, jumpSpeed;
        float moveSpeed;

        /* MECHANICS */
        bool canShoot;
        sf::Int32 shootCooldown;
};
