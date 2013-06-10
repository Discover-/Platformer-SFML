#include "movingtile.h"
#include "shareddefines.h"
#include "game.h"

MovingTile::MovingTile(Game* _game, sf::RenderWindow* _window, sf::Texture _image, int _velocity, sf::Vector2f _startPosition, sf::Vector2f _destination, bool _movesVertical) :
Tile(_game, _window, _image, _startPosition, TYPEID_MOVING_TILE)
{
    velocity = _velocity;
    movingToActualDest = true;
    movesVertical = _movesVertical;
    startPosition = _startPosition;
    destination = _destination;
}

MovingTile::~MovingTile()
{

}

void MovingTile::Update()
{
    Tile::Update();

    if (GAME_STATE_PAUSED(GetGame()->GetGameState()))
        return;

    if (movesVertical)
    {
        if (movingToActualDest)
        {
            SetPositionX(GetPositionX() + velocity);

            if (GetPositionX() > destination.x)
                movingToActualDest = false;
        }
        else
        {
            SetPositionX(GetPositionX() - velocity);

            if (GetPositionX() < startPosition.x)
                movingToActualDest = true;
        }

        for (std::list<Unit*>::iterator itr = passengers.begin(); itr != passengers.end(); ++itr)
        {
            float newPosX = movingToActualDest ? (*itr)->GetPositionX() + velocity : (*itr)->GetPositionX() - velocity;

            if (!(*itr)->CollidesWithGameobjects(newPosX, (*itr)->GetPositionY()))
                (*itr)->SetPositionX(newPosX);
        }
    }
    else
    {
        if (movingToActualDest)
        {
            SetPositionY(GetPositionY() - velocity);

            if (GetPositionY() < destination.y)
                movingToActualDest = false;
        }
        else
        {
            SetPositionY(GetPositionY() + velocity);

            if (GetPositionY() > startPosition.y)
                movingToActualDest = true;
        }

        for (std::list<Unit*>::iterator itr = passengers.begin(); itr != passengers.end(); ++itr)
            (*itr)->SetPositionY(movingToActualDest ? (*itr)->GetPositionY() - velocity : (*itr)->GetPositionY() + velocity);
    }

    Draw(NULL, true);
}

void MovingTile::HandleTimers(sf::Int32 diff_time)
{
    if (IsRemoved())
        return;


}

void MovingTile::AddPassenger(Unit* unit)
{
    passengers.push_back(unit);
    unit->SetIsOnMovingTile(true);
}

void MovingTile::RemovePassenger(Unit* unit)
{
    passengers.remove(unit);
    unit->SetIsOnMovingTile(false);
}

bool MovingTile::HasPassenger(Unit* unit)
{
    std::list<Unit*>::iterator itr = std::find(passengers.begin(), passengers.end(), unit);
    return itr != passengers.end();
}
