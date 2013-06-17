#include "unit.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "game.h"
#include "tile.h"
#include "movingtile.h"
#include "bouncetile.h"
#include "bonustile.h"

Unit::Unit(Game* _game, sf::RenderWindow* _window, sf::Vector2f position, std::vector<std::pair<int, sf::Texture>> _spritesLeft, std::vector<std::pair<int, sf::Texture>> _spritesRight, UnitTypeId _typeId, int _life, int _totalMoveFrames, int _frameInterval, bool _canFly, sf::Texture _bulletTexture)
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
    maxLifes = _life;
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
    showLifeBar = false;
    showLifeBarTimer = 0;
    shootCooldown = 0;
    bounceToLeft = false;
    bulletTexture = _bulletTexture;

    switch (_typeId)
    {
        case TYPEID_PLAYER:
            moveSpeed = 7.0f;
            isMoving = false;
            break;
        case TYPEID_ENEMY:
            moveSpeed = 3.0f;
            isMoving = true;
            lifeBarRed.setSize(sf::Vector2f(75.0f, 12.0f));
            lifeBarRed.setFillColor(sf::Color::Red);
            lifeBarRed.setPosition(position.x - lifeBarRed.getLocalBounds().width / 2.0f, position.y - lifeBarRed.getLocalBounds().height / 2.0f);
            lifeBarRed.setOutlineColor(sf::Color::Black);
            lifeBarRed.setOutlineThickness(1.0f);
            lifeBarGreen.setSize(sf::Vector2f(75.0f, 12.0f));
            lifeBarGreen.setFillColor(sf::Color::Green);
            lifeBarGreen.setPosition(position.x - lifeBarRed.getLocalBounds().width / 2.0f, position.y - lifeBarRed.getLocalBounds().height / 2.0f);
            lifeBarGreen.setOutlineColor(sf::Color::Black);
            lifeBarGreen.setOutlineThickness(1.0f);
            break;
        case TYPEID_MENU_PLAYER:
            moveSpeed = 7.0f;
            isMoving = true;
            break;
    }
}

Unit::~Unit()
{

}

void Unit::Update()
{
    if (typeId == TYPEID_MENU_PLAYER || (GAME_STATE_PAUSED(game->GetGameState()) && isAlive))
    {
        Draw(NULL, typeId == TYPEID_MENU_PLAYER);
        return;
    }

    if (game->GetGameState() == STATE_MAIN_MENU || !isAlive)
        return;

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
        if (!isOnMovingTile)
        {
            if ((!fallSpeed && !CollidesWithGameobjects(GetPositionX(), GetPositionY() + 1.0f)) || (fallSpeed && !CollidesWithGameobjects(GetPositionX(), GetPositionY() + fallSpeed)))
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
        else
        {
            isJumping = false;
            isFalling = false;
            fallSpeed = 0;
        }
    }

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

    if (typeId == TYPEID_ENEMY && showLifeBar && isAlive)
    {
        sf::FloatRect spriteRect = GetSpriteBody().getGlobalBounds();
        lifeBarGreen.setPosition(GetPositionX() - spriteRect.width / 2.0f, GetPositionY() - spriteRect.height / 2.0f);
        lifeBarRed.setPosition(GetPositionX() - spriteRect.width / 2.0f, GetPositionY() - spriteRect.height / 2.0f);
        window->draw(lifeBarRed);
        window->draw(lifeBarGreen);
    }
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

    if (typeId == TYPEID_ENEMY)
    {
        if (showLifeBar)
        {
            if (diff_time >= showLifeBarTimer)
            {
                showLifeBarTimer = 0;
                showLifeBar = false;
            }
            else
                showLifeBarTimer -= diff_time;
        }
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

    std::vector<SpecialTile*> allSpecialTiles = game->GetSpecialTiles();

    for (std::vector<SpecialTile*>::iterator itr = allSpecialTiles.begin(); itr != allSpecialTiles.end(); ++itr)
    {
        sf::FloatRect tileRect = (*itr)->GetSpriteTile().getGlobalBounds();

        if (WillCollision(spritePos.x, spritePos.y, spriteRect.height, spriteRect.width, (*itr)->GetPositionX(), (*itr)->GetPositionY(), tileRect.height, tileRect.width))
        {
            if ((*itr)->OnCollision(this))
                return true;
        }
        else
            (*itr)->OnCollisionOut(this);
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
    game->AddBullet(new Bullet(game, window, movingToLeft ? GetPositionX() + 50.0f : GetPositionX() - 15.0f, GetPositionY() + 25.0f, bulletTexture, movingToLeft));
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

     switch (typeId)
     {
         case TYPEID_PLAYER:
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
             break;
         }
         case TYPEID_ENEMY:
         {
             lifeBarGreen.setSize(sf::Vector2f(lifeBarRed.getLocalBounds().width - ((lifeBarRed.getLocalBounds().width / maxLifes) * (maxLifes - life)), 12.0f));
             showLifeBarTimer = 2500;
             showLifeBar = true;
             break;
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
