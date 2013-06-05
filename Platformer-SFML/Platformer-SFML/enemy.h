#ifndef __ENEMY_H
#define __ENEMY_H

#include "unit.h"

class Game;

class Enemy : public Unit
{
    public:
        Enemy(Game* _game, sf::RenderWindow* _window, float x1, float y1, float x2, float y2, sf::Sprite _spriteBody, TypeId _typeId);
        ~Enemy();

        void Update();
        void HandleTimers(sf::Int32 diff_time);

    private:
        Game* game;
        float destinationX1, destinationY1;
        float destinationX2, destinationY2;
        bool movingForward;
};

#endif
