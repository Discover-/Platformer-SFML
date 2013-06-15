#include "coin.h"
#include "shareddefines.h"
#include "game.h"
#include "player.h"

Coin::Coin(Game* _game, sf::RenderWindow* _window, sf::Vector2f _startPosition, sf::Texture _image)
{
    game = _game;
    image = _image;
    window = _window;
    startPosition = _startPosition;
    SetPosition(startPosition.x, startPosition.y);
    movingUp = true;
    isTaken = false;
}

Coin::~Coin()
{

}

void Coin::Update()
{
    if (!GAME_STATE_PAUSED(game->GetGameState()))
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

    sf::Sprite spriteCoin(image);
    Draw(spriteCoin, true);
}

void Coin::Draw(sf::Sprite spriteCoin, bool updatePos /* = false */)
{
    sf::Vector2f position = GetPosition();
    sf::Vector2f positionPlr = game->GetPlayer()->GetPosition();

    if (!IsInRange(position.x, positionPlr.x, position.y, positionPlr.y, 1000.0f))
        return;

    if (updatePos)
        spriteCoin.setPosition(GetPositionX(), GetPositionY());

    if (GAME_STATE_PAUSED(game->GetGameState()))
        spriteCoin.setColor(sf::Color(255, 255, 255, 128));

    window->draw(spriteCoin);
}

sf::Sprite Coin::GetSprite()
{
    sf::Sprite spriteCoin(image);
    spriteCoin.setPosition(GetPositionX(), GetPositionY());
    return spriteCoin;
}
