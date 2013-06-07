#include "movingtile.h"
#include "shareddefines.h"
#include "game.h"

MovingTile::MovingTile(Game* _game, sf::RenderWindow* _window, int _velocity, sf::Vector2f _startPosition, sf::Vector2f _destination, bool _movesVertical)
{
    game = _game;
    image.loadFromFile("Graphics/Tiles/plank.png");
    window = _window;
    velocity = _velocity;
    movingToActualDest = true;
    movesVertical = _movesVertical;
    SetPosition(_startPosition.x, _startPosition.y);
    startPosition = _startPosition;
    destination = _destination;
}

MovingTile::~MovingTile()
{

}

void MovingTile::Update()
{
    if (!GAME_STATE_PAUSED_DRAWING(game->GetGameState()))
    {
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
                (*itr)->SetPositionX(movingToActualDest ? (*itr)->GetPositionX() + velocity : (*itr)->GetPositionX() - velocity);
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
    }

    sf::Sprite spriteTile(image);
    Draw(spriteTile, true);
}

void MovingTile::Draw(sf::Sprite spriteTile, bool updatePos /* = false */)
{
    if (updatePos)
        spriteTile.setPosition(GetPositionX(), GetPositionY());

    if (game->GetGameState() == STATE_PAUSED || game->GetGameState() == STATE_PAUSED_FOCUS)
        spriteTile.setColor(sf::Color(255, 255, 255, 128));

    window->draw(spriteTile);
}

sf::Sprite MovingTile::GetSprite()
{
    sf::Sprite spriteTile(image);
    spriteTile.setPosition(GetPositionX(), GetPositionY());
    return spriteTile;
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
