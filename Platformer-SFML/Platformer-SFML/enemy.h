#ifndef __ENEMY_H
#define __ENEMY_H

#include "unit.h"

class Game;

class Enemy : public Unit
{
    public:
        Enemy(Game* _game, sf::RenderWindow* _window, float x, float y, sf::Sprite _spriteBody);
        ~Enemy();

        void Update();
        void HandleTimers(sf::Int32 diff_time);

    private:
        Game* game;

};

#endif
