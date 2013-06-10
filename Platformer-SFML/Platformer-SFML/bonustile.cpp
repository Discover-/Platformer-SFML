#include "BonusTile.h"
#include "shareddefines.h"
#include "game.h"

BonusTile::BonusTile(Game* _game, sf::RenderWindow* _window, sf::Texture _image, sf::Vector2f _startPosition) :
Tile(_game, _window, _image, _startPosition, TYPEID_BONUS_TILE)
{
    isUsed = false;
    animating = false;
    animationFinished = false;
    movingUp = false;
    startPosition = _startPosition;
    imageUsed.loadFromFile("Graphics/Tiles/bonus_used.png");
}

BonusTile::~BonusTile()
{

}

void BonusTile::Update()
{
    Tile::Update();

    if (GAME_STATE_PAUSED(GetGame()->GetGameState()))
        return;

    if (isUsed && animating && !animationFinished)
    {
        animating = true;

        if (movingUp)
        {
            SetPositionX(GetPositionX() - 2.0f);

            if (GetPositionX() < startPosition.x - 10.0f)
                movingUp = true;
        }
        else
        {
            SetPositionX(GetPositionX() + 2.0f);

            if (GetPositionX() > startPosition.x + 10.0f)
            {
                movingUp = false;
                animating = false;
                animationFinished = true;
            }
        }
    }

    Draw(isUsed ? &imageUsed : NULL, true);
}

void BonusTile::HandleTimers(sf::Int32 diff_time)
{
    if (IsRemoved())
        return;

    Tile::HandleTimers(diff_time);
}

void BonusTile::SetIsUsed(bool val)
{
    isUsed = val;

    if (val)
    {
        animating = true;
        animationFinished = false;
    }
}
