#ifndef __UNIT_H
#define __UNIT_H

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
#include "position.h"
#include "shareddefines.h"

class Game;

class Unit : public Position
{
    public:
        Unit(Game* _game, sf::RenderWindow* _window, float x, float y, sf::Sprite _spriteBody, TypeId _typeId);
        ~Unit();

        virtual void Update();
        virtual void HandleTimers(sf::Int32 diff_time);
        void Draw(sf::Sprite* _spriteBody = NULL, bool updatePos = false);
        void SetSpriteBody(sf::Sprite sprite) { spriteBody = sprite; }
        sf::Sprite GetSpriteBody() { return spriteBody; }

        /* MOVEMENT */
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
        TypeId typeId;
        bool keysDown[4];
        sf::Sprite spriteBody;
        sf::RenderWindow* window;

        /* MOVEMENT */
        bool isJumping, isFalling;
        int fallSpeed, jumpSpeed;
        float moveSpeed;

        /* MECHANICS */
        bool canShoot;
        sf::Int32 shootCooldown;
};

#endif
