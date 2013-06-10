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
#include "position.h"
#include "shareddefines.h"

class Game;

class Unit : public Position
{
    public:
        Unit(Game* _game, sf::RenderWindow* _window, sf::Vector2f position, std::vector<std::pair<int, sf::Texture>> _spritesLeft, std::vector<std::pair<int, sf::Texture>> _spritesRight, UnitTypeId _typeId, int _life, int _totalMoveFrames, int _frameInterval, bool _canFly);
        //~Unit();

        virtual void Update();
        virtual void HandleTimers(sf::Int32 diff_time);

        void Draw(sf::Sprite* _spriteBody = NULL, bool updatePos = false);

        sf::Sprite GetSpriteBody();

        Game* &GetGame() { return game; }

        UnitTypeId GetTypeId() { return typeId; }

        /* MOVEMENT */
        void SetIsMoving(bool val) { isMoving = val; }
        bool IsMoving() { return isMoving; }

        void Jump(int jumpSpeed = 15);
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

        bool IsOnMovingTile() { return isOnMovingTile; }
        void SetIsOnMovingTile(bool val) { isOnMovingTile = val; }

        bool IsInQuickSandArea() { return isInQuickSandArea; }
        void SetIsInQuickSandArea(bool val) { isInQuickSandArea = val; }
        bool IsInWaterArea() { return isInWaterArea; }
        void SetIsInWaterArea(bool val) { isInWaterArea = val; }
        bool IsInLavaArea() { return isInLavaArea; }
        void SetIsInLavaArea(bool val) { isInLavaArea = val; }

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
        void JustRespawned() { isAlive = true; }
        bool IsDead() { return !isAlive; }

        void SetCanFly(bool val) { canFly = val; }
        bool CanFly() { return canFly; }

    private:
        Game* game;
        UnitTypeId typeId;
        bool isAlive;
        std::vector<std::pair<int, sf::Texture>> spriteBodiesLeft;
        std::vector<std::pair<int, sf::Texture>> spriteBodiesRight;
        sf::RenderWindow* window;
        sf::Texture imageDeadSprite;

        /* MOVEMENT */
        bool isMoving, isJumping, isFalling, hasBounced;
        int fallSpeed, jumpSpeed, bounceSpeed, bounceToLeft;
        float moveSpeed;
        int moveFrame, totalMoveFrames, frameInterval, frameIntervalStore;
        bool canFly, movingToLeft, isOnMovingTile;
        bool isInQuickSandArea, isInWaterArea, isInLavaArea;

        /* MECHANICS */
        bool canShoot;
        sf::Int32 shootCooldown;
        int life;
};


