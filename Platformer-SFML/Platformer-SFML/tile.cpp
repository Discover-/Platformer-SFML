#include "tile.h"
#include "unit.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "game.h"

SpecialTile::SpecialTile(Game* _game, sf::RenderWindow* _window, sf::Texture _image, sf::Vector2f position, TileTypeId _typeId)
{
    window = _window;
    SetPosition(position.x, position.y);
    image = _image;
    game = _game;
    typeId = _typeId;
    isRemoved = false;
}

void SpecialTile::Update()
{
    if (GAME_STATE_PAUSED(game->GetGameState()) && !isRemoved)
    {
        Draw();
        return;
    }

    if (game->GetGameState() == STATE_MAIN_MENU)
        return;

    //Draw(NULL, true);
}

void SpecialTile::Draw(sf::Texture* _textureTile /* = NULL */, bool updatePos /* = false */)
{
    sf::Vector2f position = GetPosition();
    sf::Vector2f positionPlr = game->GetPlayer()->GetPosition();

    if (!IsInRange(position.x, positionPlr.x, position.y, positionPlr.y, 1000.0f))
        return;

    sf::Sprite spriteToDraw;

    if (!_textureTile)
        spriteToDraw = GetSpriteTile();
    else
        spriteToDraw.setTexture(*_textureTile);

    if (updatePos)
        spriteToDraw.setPosition(GetPositionX(), GetPositionY());

    if (GAME_STATE_PAUSED(game->GetGameState()))
        spriteToDraw.setColor(sf::Color(255, 255, 255, 128));

    window->draw(spriteToDraw);
}

void SpecialTile::HandleTimers(sf::Int32 diff_time)
{
    if (isRemoved)
        return;


}

sf::Sprite SpecialTile::GetSpriteTile()
{
    sf::Sprite sprite(image);
    sprite.setPosition(GetPositionX(), GetPositionY());
    return sprite;
}
