#include "bouncetile.h"
#include "shareddefines.h"
#include "game.h"
#include "player.h"

BounceTile::BounceTile(Game* _game, sf::RenderWindow* _window, sf::Texture _image, sf::Vector2f _startPosition, std::string color) :
SpecialTile(_game, _window, _image, _startPosition, TYPEID_BOUNCE_TILE)
{
    isUsed = false;
    startPosition = _startPosition;
    SetPosition(startPosition.x, startPosition.y);
    imageUsed.loadFromFile("Graphics/Tiles/switch_" + color + "_off.png");
}

BounceTile::~BounceTile()
{

}

void BounceTile::Update()
{
    SpecialTile::Update();

    if (GAME_STATE_PAUSED(GetGame()->GetGameState()))
    {
        Draw(isUsed ? &imageUsed : NULL, true);
        return;
    }

    Draw(isUsed ? &imageUsed : NULL, true);
}

void BounceTile::HandleTimers(sf::Int32 diff_time)
{
    if (IsRemoved())
        return;

    SpecialTile::HandleTimers(diff_time);
}

bool BounceTile::OnCollision(Unit* unit /* = NULL */)
{
    if (!isUsed && unit && unit->GetTypeId() == TYPEID_PLAYER)
    {
        isUsed = true;
        unit->Jump(30);
    }

    return false;
}

void BounceTile::OnCollisionOut(Unit* unit)
{

}
