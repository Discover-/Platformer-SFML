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
#include "Windows.h"
#include "position.h"
#include "shareddefines.h"

class Game;

class Unit : public Position
{
    public:
        Unit(Game* _game, sf::RenderWindow* _window, float x, float y, std::vector<std::pair<int, sf::Texture>> _spritesLeft, std::vector<std::pair<int, sf::Texture>> _spritesRight, TypeId _typeId, int _life, int _totalMoveFrames, int _frameInterval, bool _canFly);
        //~Unit();

        virtual void Update();
        virtual void HandleTimers(sf::Int32 diff_time);
        void Draw(sf::Sprite* _spriteBody = NULL, bool updatePos = false);

        //void SetSpriteBody(sf::Sprite sprite) { spriteCharacters[moveFrame].second = sprite; }
        sf::Sprite GetSpriteBody();

        /* MOVEMENT */
        void SetIsMoving(bool val) { isMoving = val; }
        bool IsMoving() { return isMoving; }

        void SetIsJumping(bool val) { isJumping = val; fallSpeed = 0; }
        bool IsJumping() { return isJumping; }
        bool IsFalling() { return isFalling; }
        bool IsBouncing() { return hasBounced; }
        void BounceAway(bool toLeft);

        bool CollidesWithGameobjects(float newPosX = 0.0f, float newPosY = 0.0f);

        float GetMoveSpeed() { return moveSpeed; }
        void SetMoveSpeed(float val) { moveSpeed = val; }

        int GetMoveFrame();
        bool IsMovingToLeft() { return movingToLeft; }
        void SetIsMovingToLeft(bool val) { movingToLeft = val; }

        /* MECHANICS */
        void Shoot();
        bool CanShoot() { return canShoot; }
        void SetCanShoot(bool val) { canShoot = val; }
        void SetShootCooldown(sf::Int32 val) { shootCooldown = val; }

        //! DropLife returns true if there are no lifes left
        bool DropLife();
        void SetLife(int val) { life = val; }
        int GetLife() { return life; }

        void JustDied() { isAlive = false; }
        bool IsDead() { return !isAlive; }

        void SetCanFly(bool val) { canFly = val; }
        bool CanFly() { return canFly; }

    private:
        Game* game;
        TypeId typeId;
        bool isAlive;
        std::vector<std::pair<int, sf::Texture>> spriteBodiesLeft;
        std::vector<std::pair<int, sf::Texture>> spriteBodiesRight;
        sf::RenderWindow* window;

        /* MOVEMENT */
        bool isMoving, isJumping, isFalling, hasBounced;
        int fallSpeed, jumpSpeed, bounceSpeed, bounceToLeft;
        float moveSpeed;
        int moveFrame, totalMoveFrames, frameInterval, frameIntervalStore;
        bool canFly;
        bool movingToLeft;

        /* MECHANICS */
        bool canShoot;
        sf::Int32 shootCooldown;
        int life;
};


