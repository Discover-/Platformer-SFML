#include "movingtile.h"
#include "shareddefines.h"
#include "game.h"

MovingTile::MovingTile(Game* _game, sf::RenderWindow* _window, sf::Texture _image, int _velocity, sf::Vector2f _startPosition, sf::Vector2f _destination)
{
    game = _game;
    image = _image;
    window = _window;
    velocity = _velocity;
    movingToLeft = true;
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
        if (movingToLeft)
        {
            SetPositionX(GetPositionX() + velocity);

            if (GetPositionX() > destination.x)
                movingToLeft = false;
        }
        else
        {
            SetPositionX(GetPositionX() - velocity);

            if (GetPositionX() < startPosition.x)
                movingToLeft = true;
        }

        for (std::list<Unit*>::iterator itr = passengers.begin(); itr != passengers.end(); ++itr)
            (*itr)->SetPositionX(movingToLeft ? (*itr)->GetPositionX() + velocity : (*itr)->GetPositionX() - velocity);
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
