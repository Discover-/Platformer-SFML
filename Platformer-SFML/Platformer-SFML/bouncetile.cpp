#include "BounceTile.h"
#include "shareddefines.h"
#include "game.h"

BounceTile::BounceTile(Game* _game, sf::RenderWindow* _window, sf::Texture _image, int _velocity, sf::Vector2f _startPosition, sf::Vector2f _destination, std::string color) :
Tile(_game, _window, _image, _startPosition, TYPEID_BOUNCE_TILE)
{
    isUsed = false;
    startPosition = _startPosition;
    destination = _destination;
    imageUsed.loadFromFile("Graphics/Tiles/switch_" + color + "_on.png");
}

BounceTile::~BounceTile()
{

}

void BounceTile::Update()
{
    Tile::Update();

    if (GAME_STATE_PAUSED(GetGame()->GetGameState()))
        return;

    Draw(isUsed ? &imageUsed : NULL, true);
}

void BounceTile::HandleTimers(sf::Int32 diff_time)
{
    if (IsRemoved())
        return;

    Tile::HandleTimers(diff_time);
}
