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
    _spriteBullet.setPosition(_x, _y);
    spriteBullet = _spriteBullet;
    //spriteBullet.setTexture(_spriteBullet);
    spriteBullet.setPosition(_x, _y);
    //sf::Texture imageBullet;
    //imageBullet.loadFromFile("tux_frame_0.png");
    //spriteBullet.setTexture(imageBullet);
    //spriteBullet.setPosition(posX, posY);
    _spriteBullet.setPosition(_x - 200, _y);
    window->draw(_spriteBullet);
    window->draw(spriteBullet);
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

    if (Collision::PixelPerfectTest(spriteBullet, spriteChar))
        Explode();
    //else
    {
        //std::vector<sf::Sprite> gameObjects = game->GetGameObjectsCollidable();

        //for (std::vector<sf::Sprite>::iterator itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
        //{
        //    sf::Vector2f posGo = (*itr).getPosition();
        //    //std::cout << "Gameobject all X: " << posGo.x << ", Y: " << posGo.y << std::endl;

        //    if (IsInRange(posX, posGo.x, posY, posGo.y, 200.0f))
        //    {
        //        std::cout << "Gameobject inrange X: " << posGo.x << ", Y: " << posGo.y << std::endl;

        //        if (Collision::PixelPerfectTest(spriteBullet, (*itr)))
        //        {
        //            Explode();
        //            break;
        //        }
        //    }
        //}
    }

    if (posX < 0)
        SetPosX(0.0f);

    if (posY < 0)
        SetPosY(0.0f);

    Draw(NULL, true);
}

void Bullet::Draw(sf::Sprite* _spriteBullet /* = NULL */, bool updatePos /* = false */)
{
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
