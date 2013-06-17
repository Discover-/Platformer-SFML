#include "floatingtile.h"
#include "shareddefines.h"
#include "game.h"
#include "player.h"

FloatingTile::FloatingTile(sf::Vector2f _startPosition)
{
    startPosition = _startPosition;
    SetPosition(startPosition.x, startPosition.y);
    movingUp = true;
}

FloatingTile::~FloatingTile()
{

}

void FloatingTile::Update()
{
    if (movingUp)
    {
        SetPositionY(GetPositionY() - 1.0f);

        if (GetPositionY() < startPosition.y - 10.0f)
            movingUp = false;
    }
    else
    {
        SetPositionY(GetPositionY() + 1.0f);

        if (GetPositionY() > startPosition.y + 10.0f)
            movingUp = true;
    }
}
