//#include "unit.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "collision.h"
#include "game.h"

Unit::Unit(Game* _game, sf::RenderWindow* _window, float x, float y, sf::Sprite _spriteBody)
{
    window = _window;
    SetPositionX(x);
    SetPositionY(y);
    game = _game;
    spriteBody = _spriteBody;

    for (int i = 0; i < 4; ++i)
        keysDown[i] = false;

    isJumping = false;
    isFalling = true;
    fallSpeed = 0;
    jumpSpeed = 15;
    moveSpeed = 10.0f;
}

void Unit::Update()
{
    if (isJumping)
    {
        if (jumpSpeed && !CollidesWithGameobjects(GetPositionX(), GetPositionY() - jumpSpeed))
        {
            SetPositionY(GetPositionY() - jumpSpeed);
            jumpSpeed--;
        }
        else
        {
            isJumping = false;
            isFalling = true;
            jumpSpeed = 15;
        }
    }

    if (isFalling)
    {
        if (!CollidesWithGameobjects(GetPositionX(), GetPositionY() + fallSpeed + 5.0f))
        {
            SetPositionY(GetPositionY() + fallSpeed);
            fallSpeed++;
        }
        else
        {
            //SetPositionY(posY + 5.0f);
            isFalling = false;
            fallSpeed = 0;
            jumpSpeed = 15;
        }
    }

    if (GetPositionX() < 0)
        SetPositionX(0.0f);

    if (GetPositionY() < 0)
        SetPositionY(0.0f);

    Draw(NULL, true);
}

void Unit::Draw(sf::Sprite* _spriteBody /* = NULL */, bool updatePos /* = false */)
{
    sf::Sprite* spriteToDraw = _spriteBody ? _spriteBody : &spriteBody;

    if (updatePos)
        spriteToDraw->setPosition(GetPositionX(), GetPositionY());

    window->draw(*spriteToDraw);
}

void Unit::HandleTimers(sf::Int32 diff_time)
{
    if (!canShoot)
    {
        if (diff_time >= shootCooldown)
        {
            shootCooldown = 0;
            canShoot = true;
        }
        else
            shootCooldown -= diff_time;
    }
}

bool Unit::CollidesWithGameobjects(float newPosX /* = 0.0f */, float newPosY /* = 0.0f */)
{
    sf::Sprite charSprite = spriteBody;

    if (newPosX != 0.0f && newPosY != 0.0f)
        charSprite.setPosition(newPosX, newPosY);

    std::vector<sf::Sprite> gameObjects = game->GetGameObjectsCollidable();
    for (std::vector<sf::Sprite>::iterator itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
        if (Collision::PixelPerfectTest(charSprite, (*itr)))
            return true;

    return false;
}

void Unit::Shoot()
{
    shootCooldown = 400;
    canShoot = false;
    //Bullet* bullet = new Bullet(game, window, posX + 50, posY + 20);

    sf::Texture imageBullet;
    imageBullet.loadFromFile("tux_frame_0.png");
    sf::Sprite spriteBullet(imageBullet);
    Bullet* bullet = new Bullet(game, window, GetPositionX() + 150, GetPositionY() + 20, spriteBullet);
    game->AddBullet(bullet);
}
