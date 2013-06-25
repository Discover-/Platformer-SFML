#include "spiketile.h"
#include "shareddefines.h"
#include "game.h"
#include "player.h"

SpikeTile::SpikeTile(sf::RenderWindow* _window, sf::Texture _image, sf::Vector2f _startPosition) :
SpecialTile(_window, _image, _startPosition, TYPEID_LAVA_TILE)
{
    SetPosition(_startPosition.x, _startPosition.y);
}

SpikeTile::~SpikeTile()
{

}

void SpikeTile::Update()
{
    SpecialTile::Update();
    Draw(NULL, true);
}

void SpikeTile::HandleTimers(sf::Int32 diff_time)
{
    if (IsRemoved())
        return;

    SpecialTile::HandleTimers(diff_time);
}

bool SpikeTile::OnCollision(Unit* unit /* = NULL */)
{
    if (unit && unit->GetTypeId() == TYPEID_PLAYER)
        unit->BounceAway(unit->IsMovingToLeft());

    return false;
}

void SpikeTile::OnCollisionOut(Unit* unit)
{
    return;
}
