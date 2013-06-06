#include "bullet.h"
#include "player.h"
#include "collision.h"
#include "game.h"
#include "shareddefines.h"

Bullet::Bullet(Game* _game, sf::RenderWindow* _window, float _x, float _y, sf::Sprite _spriteBullet, float _velocity /* = 5 */)
{
    if (!_game)
        return;

    game = _game;
    velocity = _velocity;
    isRemoved = false;
    posX = _x;
    posY = _y;
    window = _window;
    spriteBullet = _spriteBullet;
    spriteBullet.setPosition(_x, _y);
}

Bullet::~Bullet()
{

}

void Bullet::Update()
{
    Player* player = game->GetPlayer();

    if (isRemoved || !game || !game->IsRunning() || !player)
        return;

    //SetPosXY(posX + velocity, posY);
    posX += velocity;

    sf::Sprite spriteChar = player->GetSpriteBody();
    sf::Vector2f posBullet = spriteBullet.getPosition();
    sf::Vector2f posChar = spriteChar.getPosition();
    sf::FloatRect boundsBullet = spriteBullet.getGlobalBounds();
    sf::FloatRect boundsChar = spriteChar.getGlobalBounds();

    //if (Collision::PixelPerfectTest(spriteBullet, spriteChar))
    if (WillCollision(posBullet.x, posBullet.y, boundsBullet.height, boundsBullet.width, posChar.x, posChar.y, boundsChar.height, boundsChar.width))
        Explode();
    else
    {
        std::vector<sf::Sprite> gameObjects = game->GetGameObjectsCollidable();

        for (std::vector<sf::Sprite>::iterator itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
        {
            sf::Vector2f posGameobject = (*itr).getPosition();
            sf::FloatRect boundsGameobject = spriteBullet.getGlobalBounds();

            if (IsInRange(posX, posGameobject.x, posY, posGameobject.y, 200.0f))
            {
                if (WillCollision(posBullet.x, posBullet.y, boundsBullet.height, boundsBullet.width, posGameobject.x, posGameobject.y, boundsGameobject.height, boundsGameobject.width))
                {
                    Explode();
                    break;
                }
            }
        }
    }

    if (posX < 0)
        SetPosX(0.0f);

    if (posY < 0)
        SetPosY(0.0f);

    Draw(NULL, true);
}

void Bullet::Draw(sf::Sprite* _spriteBullet /* = NULL */, bool updatePos /* = false */)
{
    if (isRemoved)
        return;

    sf::Sprite* spriteToDraw = _spriteBullet ? _spriteBullet : &spriteBullet;

    if (updatePos)
        spriteToDraw->setPosition(posX, posY);

    window->draw(*spriteToDraw);
}

void Bullet::Explode()
{
    isRemoved = true;
}

void Bullet::SetPosX(float val)
{
    SetPosXY(val, posY);
}

void Bullet::SetPosY(float val)
{
    SetPosXY(posX, val);
}

void Bullet::SetPosXY(float valX, float valY)
{
    posX = valX;
    posY = valY;
    spriteBullet.setPosition(posX, posY);
}
