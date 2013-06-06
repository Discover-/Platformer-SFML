#include "unit.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "collision.h"
#include "game.h"

Unit::Unit(Game* _game, sf::RenderWindow* _window, float x, float y, std::vector<std::pair<int, sf::Texture>> _spriteBodies, TypeId _typeId, int _life, int _totalMoveFrames, int _frameInterval, bool _canFly)
{
    window = _window;
    SetPositionX(x);
    SetPositionY(y);
    game = _game;
    spriteBodies = _spriteBodies;
    typeId = _typeId;
    isJumping = false;
    isMoving = _typeId == TYPEID_ENEMY;
    fallSpeed = 0;
    jumpSpeed = 15;
    bounceSpeed = 15;
    moveSpeed = _typeId == TYPEID_PLAYER ? 7.0f : 3.0f;
    life = _life;
    moveFrame = 0;
    totalMoveFrames = _totalMoveFrames;
    frameInterval = 0;
    frameIntervalStore = _frameInterval;
    canFly = _canFly;
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
    //else if (isBouncing)
    //{
    //    float newX = bounceToLeft ? GetPositionX() - bounceSpeed : GetPositionX() + bounceSpeed;

    //    if (bounceSpeed)// && !CollidesWithGameobjects(newX, GetPositionY()))
    //    {
    //        SetPositionY(newX);
    //        bounceSpeed--;
    //    }
    //    else
    //    {
    //        isBouncing = false;
    //        isJumping = false;
    //        isFalling = true;
    //        bounceSpeed = 15;
    //        fallSpeed = 0;
    //    }
    //}
    else if (!canFly)
    {
        if (!CollidesWithGameobjects(GetPositionX(), GetPositionY() + fallSpeed))
        {
            isFalling = true;
            SetPositionY(GetPositionY() + fallSpeed);
            fallSpeed++;
        }
        else
        {
            isFalling = false;
            fallSpeed = 0;
        }
    }

    //if (!isFalling && !isJumping && !isBouncing)
    //    BounceAway(urand(0, 1) == 0 ? false : true);

    if (GetPositionX() < 0)
        SetPositionX(0.0f);

    if (GetPositionY() < 0)
        SetPositionY(0.0f);

    Draw(NULL, true);
}

void Unit::Draw(sf::Sprite* _spriteBody /* = NULL */, bool updatePos /* = false */)
{
    sf::Sprite spriteToDraw = _spriteBody ? *_spriteBody : GetSpriteBody();

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

    //if (!isJumping && !isFalling)
    {
        if (isMoving)
        {
            if (diff_time >= frameInterval)
            {
                frameInterval = frameIntervalStore;
                moveFrame++;

                if (moveFrame > totalMoveFrames)
                    moveFrame = typeId == TYPEID_PLAYER ? 1 : 0;
            }
            else
                frameInterval -= diff_time;
        }
        else if (!isMoving)
            moveFrame = 0;
    }
}

bool Unit::CollidesWithGameobjects(float newPosX /* = 0.0f */, float newPosY /* = 0.0f */)
{
    sf::Sprite spriteBody = GetSpriteBody();

    if (newPosX != 0.0f && newPosY != 0.0f)
        spriteBody.setPosition(newPosX, newPosY);

    sf::Vector2f spritePos = spriteBody.getPosition();
    sf::FloatRect spriteRect = spriteBody.getGlobalBounds();

    std::vector<sf::Sprite> gameObjects = game->GetGameObjectsCollidable();
    for (std::vector<sf::Sprite>::iterator itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
    {
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

    sf::Texture imageBullet;
    imageBullet.loadFromFile("Graphics/Other/bullet.png");
    Bullet* bullet = new Bullet(game, window, GetPositionX() + 50, GetPositionY() + 20, imageBullet);
    game->AddBullet(bullet);
}

void Unit::BounceAway(bool toLeft)
{
    isJumping = false;
    //isBouncing = true;
    bounceToLeft = toLeft;
    fallSpeed = 0;
}

sf::Sprite Unit::GetSpriteBody()
{
    for (std::vector<std::pair<int, sf::Texture>>::iterator itr = spriteBodies.begin(); itr != spriteBodies.end(); ++itr)
    {
        if ((*itr).first == moveFrame)
        {
            sf::Sprite sprite((*itr).second);
            sprite.setPosition(GetPositionX(), GetPositionY());
            return sprite;
        }
    }

    return sf::Sprite();
}
