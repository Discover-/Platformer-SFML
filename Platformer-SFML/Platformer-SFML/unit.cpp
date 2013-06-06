#include "unit.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "collision.h"
#include "game.h"

Unit::Unit(Game* _game, sf::RenderWindow* _window, float x, float y, sf::Sprite _spriteBody, TypeId _typeId, int _life)
{
    sf::Vector2f vec = _spriteBody.getPosition();
    float __x = vec.x;
    float __y = vec.y;
    sf::Texture const* tex = _spriteBody.getTexture();
    window = _window;
    SetPositionX(x);
    SetPositionY(y);
    game = _game;
    spriteBody = _spriteBody;
    typeId = _typeId;
    isJumping = false;
    fallSpeed = 0;
    jumpSpeed = 15;
    moveSpeed = _typeId == TYPEID_PLAYER ? 10.0f : 3.0f;
    life = _life;
}

void Unit::Update()
{
    if (!isAlive)
        return;

    if (GAME_STATE_PAUSED_DRAWING(game->GetGameState()))
    {
        Draw();
        return;
    }

    if (game->GetGameState() == STATE_MENU)
        return;

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
    else
    {
        isFalling = true;

        if (!CollidesWithGameobjects(GetPositionX(), GetPositionY() + fallSpeed + 5.0f))
        {
            SetPositionY(GetPositionY() + fallSpeed);
            fallSpeed++;
        }
        else
        {
            isFalling = false;
            fallSpeed = 0;
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
    sf::Sprite spriteToDraw = _spriteBody ? *_spriteBody : spriteBody;

    if (updatePos)
        spriteToDraw.setPosition(GetPositionX(), GetPositionY());

    if (GAME_STATE_PAUSED_DRAWING(game->GetGameState()))
        spriteToDraw.setColor(sf::Color(255, 255, 255, 128));

    window->draw(spriteToDraw);
}

void Unit::HandleTimers(sf::Int32 diff_time)
{
    if (!isAlive)
        return;

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

    sf::Vector2f spritePos = charSprite.getPosition();
    sf::FloatRect spriteRect = charSprite.getGlobalBounds();

    std::vector<sf::Sprite> gameObjects = game->GetGameObjectsCollidable();
    for (std::vector<sf::Sprite>::iterator itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
    {
        //if (Collision::PixelPerfectTest(charSprite, (*itr)))

        sf::Vector2f gameobjectPos = (*itr).getPosition();
        sf::FloatRect gameobjectRect = (*itr).getGlobalBounds();

        if (WillCollision(spritePos.x, spritePos.y, spriteRect.height, spriteRect.width, gameobjectPos.x, gameobjectPos.y, gameobjectRect.height, gameobjectRect.width))
            return true;

    }

    return false;
}

void Unit::Shoot()
{
    if (!isAlive)
        return;

    shootCooldown = 400;
    canShoot = false;
    //Bullet* bullet = new Bullet(game, window, posX + 50, posY + 20);

    sf::Texture imageBullet;
    imageBullet.loadFromFile("Graphics/Other/bullet.png");
    Bullet* bullet = new Bullet(game, window, GetPositionX() + 50, GetPositionY() + 20, imageBullet);
    game->AddBullet(bullet);
}
