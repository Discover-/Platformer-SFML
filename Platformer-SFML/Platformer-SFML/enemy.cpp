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
#include "enemy.h"
#include "collision.h"
#include "shareddefines.h"
#include "bullet.h"
#include "unit.h"

Enemy::Enemy(Game* _game, sf::RenderWindow* _window, float x, float y, sf::Sprite _spriteBody) : Unit(_game, _window, x, y, _spriteBody)
{

}

void Enemy::Update()
{
    Unit::Update();
}

void Enemy::HandleTimers(sf::Int32 diff_time)
{
    Unit::HandleTimers(diff_time);
}
