#include "watertile.h"
#include "shareddefines.h"
#include "game.h"
#include "player.h"

WaterTile::WaterTile(Game* _game, sf::RenderWindow* _window, sf::Texture _image, sf::Vector2f _startPosition) :
SpecialTile(_game, _window, _image, _startPosition, TYPEID_WATER_TILE)
{
    SetPosition(_startPosition.x, _startPosition.y);
}

WaterTile::~WaterTile()
{

}

void WaterTile::Update()
{
    SpecialTile::Update();
    Draw(NULL, true);
}

void WaterTile::HandleTimers(sf::Int32 diff_time)
{
    if (IsRemoved())
        return;

    SpecialTile::HandleTimers(diff_time);
}

bool WaterTile::OnCollision(Unit* unit /* = NULL */)
{
    if (unit && !unit->IsInWaterArea())
    {
        unit->SetIsInWaterArea(true);
        unit->SetMoveSpeed(unit->GetMoveSpeed() / 2.0f);
    }

    return false;
}

void WaterTile::OnCollisionOut(Unit* unit)
{
    if (unit->IsInWaterArea())
    {
        unit->SetIsInWaterArea(false);
        unit->SetMoveSpeed(unit->GetMoveSpeed() * 2.0f);
    }
}
