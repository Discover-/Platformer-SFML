#include "lavatile.h"
#include "shareddefines.h"
#include "game.h"
#include "player.h"

LavaTile::LavaTile(sf::RenderWindow* _window, sf::Texture _image, sf::Vector2f _startPosition) :
SpecialTile(_window, _image, _startPosition, TYPEID_LAVA_TILE)
{
    SetPosition(_startPosition.x, _startPosition.y);
}

LavaTile::~LavaTile()
{

}

void LavaTile::Update()
{
    SpecialTile::Update();
    Draw(NULL, true);
}

void LavaTile::HandleTimers(sf::Int32 diff_time)
{
    if (IsRemoved())
        return;

    SpecialTile::HandleTimers(diff_time);
}

bool LavaTile::OnCollision(Unit* unit /* = NULL */)
{
    if (unit && unit->GetTypeId() == TYPEID_PLAYER)
    {
        unit->SetIsInLavaArea(true);
        unit->BounceAway(unit->IsMovingToLeft());
    }

    return false;
}

void LavaTile::OnCollisionOut(Unit* unit)
{
    unit->SetIsInLavaArea(false);
}
