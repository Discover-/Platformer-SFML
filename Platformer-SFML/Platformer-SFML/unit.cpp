#include "unit.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "game.h"
#include "tile.h"
#include "movingtile.h"
#include "bouncetile.h"
#include "bonustile.h"

Unit::Unit(Game* _game, sf::RenderWindow* _window, sf::Vector2f position, std::vector<std::pair<int, sf::Texture>> _spritesLeft, std::vector<std::pair<int, sf::Texture>> _spritesRight, UnitTypeId _typeId, int _life, int _totalMoveFrames, int _frameInterval, bool _canFly)
{
    window = _window;
    SetPosition(position.x, position.y);
    game = _game;
    spriteBodiesLeft = _spritesLeft;
    spriteBodiesRight = _spritesRight;
    typeId = _typeId;
    isJumping = false;
    fallSpeed = 0;
    jumpSpeed = 15;
    bounceSpeed = 15;
    life = _life;
    moveFrame = 0;
    totalMoveFrames = _totalMoveFrames;
    frameInterval = 0;
    frameIntervalStore = _frameInterval;
    canFly = _canFly;
    hasBounced = false;
    isOnMovingTile = false;
    isInQuickSandArea = false;
    isInWaterArea = false;
    isInLavaArea = false;

    switch (_typeId)
    {
        case TYPEID_PLAYER:
            imageJumpSprite.loadFromFile("Graphics/Character/jump.png");
            moveSpeed = 7.0f;
            isMoving = false;
            break;
        case TYPEID_ENEMY:
            imageDeadSprite.loadFromFile("Graphics/Enemies/" + std::string(canFly ? "fly_dead" : "slime_dead") + ".png");
            moveSpeed = 3.0f;
            isMoving = true;
            break;
        case TYPEID_MENU_PLAYER:
            moveSpeed = 7.0f;
            isMoving = true;
            break;
    }
}

void Unit::Update()
{
    if (typeId == TYPEID_MENU_PLAYER || (GAME_STATE_PAUSED(game->GetGameState()) && isAlive))
    {
        Draw(NULL, typeId == TYPEID_MENU_PLAYER);
        return;
    }

    if (game->GetGameState() == STATE_MAIN_MENU)
        return;

    if (!isAlive)
    {
        if (typeId == TYPEID_ENEMY)
        {
            if (canFly && GetPositionY() > 0)
            {
                SetPositionY(GetPositionY() + fallSpeed);
                fallSpeed++;

                sf::Sprite sprite(imageDeadSprite);
                Draw(&sprite, true);
            }
            else if (!canFly)
            {
                if (!CollidesWithGameobjects(GetPositionX(), GetPositionY() + fallSpeed))
                {
                    SetPositionY(GetPositionY() + fallSpeed);
                    fallSpeed++;
                }
                else
                    fallSpeed = 0;

                sf::Sprite sprite(imageDeadSprite);
                Draw(&sprite, true);
            }
        }

        return;
    }

    sf::FloatRect spriteRect = GetSpriteBody().getGlobalBounds();

    if (game->IsQuickSandArea(GetPositionX(), GetPositionY(), spriteRect.height, spriteRect.width))
    {
        if (!isInQuickSandArea)
        {
            SetMoveSpeed(GetMoveSpeed() / 2.0f);
            isInQuickSandArea = true;
        }
    }
    else if (isInQuickSandArea)
    {
        SetMoveSpeed(GetMoveSpeed() * 2.0f);
        isInQuickSandArea = false;
    }

    if (game->IsInWaterArea(GetPositionX(), GetPositionY(), spriteRect.height, spriteRect.width))
    {
        if (!isInWaterArea)
        {
            SetMoveSpeed(GetMoveSpeed() / 2.0f);
            isInWaterArea = true;
        }
    }
    else if (isInWaterArea)
    {
        SetMoveSpeed(GetMoveSpeed() * 2.0f);
        isInWaterArea = false;
    }

    if (game->IsInLavaArea(GetPositionX(), GetPositionY(), spriteRect.height, spriteRect.width))
    {
        if (!isInLavaArea)
        {
            BounceAway(IsMovingToLeft());
            isInLavaArea = true;
        }
    }
    else if (isInLavaArea)
        isInLavaArea = false;

    if (hasBounced)
    {
        float newX = bounceToLeft ? GetPositionX() - bounceSpeed : GetPositionX() + bounceSpeed;

        if (!CollidesWithGameobjects(newX, GetPositionY()))
        {
            SetPosition(newX, GetPositionY());
            bounceSpeed--;
        }

        if (bounceSpeed <= 0)
            hasBounced = false;
    }

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
    else if (!canFly)
    {
        if (!isOnMovingTile && !CollidesWithGameobjects(GetPositionX(), GetPositionY() + fallSpeed))
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

    if (GAME_STATE_PAUSED(game->GetGameState()))
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

    if (typeId == TYPEID_MENU_PLAYER || !GAME_STATE_PAUSED(game->GetGameState()))
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
        else
            moveFrame = 0;
    }
}

bool Unit::CollidesWithGameobjects(float newPosX /* = 0.0f */, float newPosY /* = 0.0f */)
{
    if (typeId == TYPEID_MENU_PLAYER)
        return false;

    sf::Sprite spriteBody = GetSpriteBody();

    if (newPosX != 0.0f && newPosY != 0.0f)
        spriteBody.setPosition(newPosX, newPosY);

    sf::Vector2f spritePos = spriteBody.getPosition();
    sf::FloatRect spriteRect = spriteBody.getGlobalBounds();

    std::vector<Tile*> allTiles = game->GetTiles();

    for (std::vector<Tile*>::iterator itr = allTiles.begin(); itr != allTiles.end(); ++itr)
    {
        sf::FloatRect tileRect = (*itr)->GetSpriteTile().getGlobalBounds();

        if (WillCollision(spritePos.x, spritePos.y, spriteRect.height, spriteRect.width, (*itr)->GetPositionX(), (*itr)->GetPositionY(), tileRect.height, tileRect.width))
            if ((*itr)->OnCollision(this))
                return true;
    }

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
    if (!isAlive || typeId == TYPEID_MENU_PLAYER)
        return;

    shootCooldown = 400;
    canShoot = false;

    sf::Texture imageBullet;
    imageBullet.loadFromFile("Graphics/Other/bullet.png");
    Bullet* bullet = new Bullet(game, window, movingToLeft ? GetPositionX() + 50.0f : GetPositionX() - 15.0f, GetPositionY() + 25.0f, imageBullet, movingToLeft);
    game->AddBullet(bullet);
}

void Unit::BounceAway(bool toLeft)
{
    if (typeId == TYPEID_MENU_PLAYER)
        return;

    Jump();
    hasBounced = true;
    bounceToLeft = toLeft;
    bounceSpeed = 15;
    fallSpeed = 0;
    DropLife();
}

sf::Sprite Unit::GetSpriteBody()
{
    if (typeId == TYPEID_PLAYER && (isJumping))// || (fallSpeed && !CollidesWithGameobjects(GetPositionX(), GetPositionY() + fallSpeed))))
    {
        sf::Sprite sprite(imageJumpSprite);
        sprite.setPosition(GetPositionX(), GetPositionY());
        return sprite;
    }

    if (movingToLeft && typeId != TYPEID_MENU_PLAYER)
    {
        for (std::vector<std::pair<int, sf::Texture>>::iterator itr = spriteBodiesLeft.begin(); itr != spriteBodiesLeft.end(); ++itr)
        {
            if ((*itr).first == moveFrame)
            {
                sf::Sprite sprite((*itr).second);
                sprite.setPosition(GetPositionX(), GetPositionY());
                return sprite;
            }
        }
    }
    else
    {
        for (std::vector<std::pair<int, sf::Texture>>::iterator itr = spriteBodiesRight.begin(); itr != spriteBodiesRight.end(); ++itr)
        {
            if ((*itr).first == moveFrame)
            {
                sf::Sprite sprite((*itr).second);
                sprite.setPosition(GetPositionX(), GetPositionY());
                return sprite;
            }
        }
    }

    return sf::Sprite();
}

bool Unit::DropLife()
{
     life--;

     if (typeId == TYPEID_PLAYER)
     {
        std::vector<std::pair<int /* id */, bool /* full */>> &hearts = ((Player*)this)->GetHearts();

        for (std::vector<std::pair<int /* id */, bool /* full */>>::iterator itr = hearts.begin(); itr != hearts.end(); ++itr)
        {
            if ((*itr).second)
            {
                (*itr).second = false;
                break;
            }
        }
     }

     return !life;
}

void Unit::Jump(int _jumpSpeed /* = 15 */)
{
    isJumping = true;
    fallSpeed = 0;
    jumpSpeed = _jumpSpeed;
}
