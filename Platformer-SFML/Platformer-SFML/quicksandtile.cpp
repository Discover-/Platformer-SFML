#include "quicksandtile.h"
#include "shareddefines.h"
#include "game.h"
#include "player.h"

QuickSandTile::QuickSandTile(Game* _game, sf::RenderWindow* _window, sf::Texture _image, sf::Vector2f _startPosition) :
SpecialTile(_game, _window, _image, _startPosition, TYPEID_QUICK_SAND_TILE)
{
    SetPosition(_startPosition.x, _startPosition.y);
}

QuickSandTile::~QuickSandTile()
{

}

void QuickSandTile::Update()
{
    SpecialTile::Update();
    Draw(NULL, true);
}

void QuickSandTile::HandleTimers(sf::Int32 diff_time)
{
    if (IsRemoved())
        return;

    SpecialTile::HandleTimers(diff_time);
}

bool QuickSandTile::OnCollision(Unit* unit /* = NULL */)
{
    if (unit && !unit->IsInQuickSandArea())
    {
        unit->SetIsInQuickSandArea(true);
        unit->SetMoveSpeed(unit->GetMoveSpeed() / 2.0f);
    }

    return true;
}

void QuickSandTile::OnCollisionOut(Unit* unit)
{
    if (unit->IsInQuickSandArea())
    {
        unit->SetIsInQuickSandArea(false);
        unit->SetMoveSpeed(unit->GetMoveSpeed() * 2.0f);
    }
}
