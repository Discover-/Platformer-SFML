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
#include "menuplayer.h"
#include "collision.h"
#include "shareddefines.h"
#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include "coin.h"
#include <math.h>

MenuPlayer::MenuPlayer(Game* _game, sf::RenderWindow* _window, sf::Vector2f position, std::vector<std::pair<int, sf::Texture>> _spritesRight, UnitTypeId _typeId, int _totalMoveFrames, int _frameInterval) :
Unit(_game, _window, position, _spritesRight, _spritesRight, _typeId, 1, _totalMoveFrames, _frameInterval, true)
{

}

void MenuPlayer::Update()
{
    Unit::Update();

    if (GetGame()->GetGameState() == STATE_MENU)
        return;

    SetIsMoving(true);
    SetIsMovingToLeft(false);
    SetPosition(GetPositionX() + GetMoveSpeed(), GetPositionY());
}

void MenuPlayer::HandleTimers(sf::Int32 diff_time)
{
    //if (GetGame()->GetGameState() != STATE_MENU)
    //    return;

    Unit::HandleTimers(diff_time);
}
