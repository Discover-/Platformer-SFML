#include "bullet.h"
#include "player.h"
#include "collision.h"
#include "game.h"
#include "shareddefines.h"
#include "enemy.h"

Bullet::Bullet(Game* _game, sf::RenderWindow* _window, float _x, float _y, sf::Texture _imageBullet, bool _movingToLeft, float _velocity /* = 5 */)
{
    if (!_game)
        return;

    game = _game;
    velocity = _velocity;
    isRemoved = false;
    SetPosition(_x, _y);
    window = _window;
    imageBullet = _imageBullet;
    movingToLeft = _movingToLeft;
}

Bullet::~Bullet()
{

}

void Bullet::Update()
{
    Player* player = game->GetPlayer();

    if (isRemoved || !game || !game->IsRunning() || !player)
        return;

    bool isPaused = GAME_STATE_PAUSED(game->GetGameState());

    if (!isPaused)
        SetPositionX(movingToLeft ? GetPositionX() + velocity : GetPositionX() - velocity);

    sf::Sprite spriteBullet(imageBullet);
    spriteBullet.setPosition(GetPosition());

    if (!isPaused)
    {
        sf::Sprite spriteChar = player->GetSpriteBody();
        sf::Vector2f posBullet = spriteBullet.getPosition();
        sf::Vector2f posChar = spriteChar.getPosition();
        sf::FloatRect boundsBullet = spriteBullet.getGlobalBounds();
        sf::FloatRect boundsChar = spriteChar.getGlobalBounds();

        if (WillCollision(GetPositionX(), GetPositionY(), boundsBullet.height, boundsBullet.width, posChar.x, posChar.y, boundsChar.height, boundsChar.width))
        {
            Explode();
            player->DropLife();
        }
        else
        {
            std::vector<Enemy*> enemies = game->GetEnemies();

            for (std::vector<Enemy*>::iterator itr = enemies.begin(); itr != enemies.end(); ++itr)
            {
                if ((*itr)->IsDead())
                    continue;

                float enemyX, enemyY;
                (*itr)->GetPosition(enemyX, enemyY);
                sf::FloatRect boundsEnemy = (*itr)->GetSpriteBody().getGlobalBounds();

                if (IsInRange(GetPositionX(), enemyX, GetPositionY(), enemyY, 200.0f))
                {
                    if (WillCollision(posBullet.x, posBullet.y, boundsBullet.height, boundsBullet.width, enemyX, enemyY, boundsEnemy.height, boundsEnemy.width))
                    {
                        Explode();

                        if ((*itr)->DropLife())
                            (*itr)->JustDied();

                        break;
                    }
                }
            }

            std::vector<sf::Sprite> gameObjects = game->GetGameObjectsCollidable();

            for (std::vector<sf::Sprite>::iterator itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
            {
                sf::Vector2f posGameobject = (*itr).getPosition();
                sf::FloatRect boundsGameobject = (*itr).getGlobalBounds();

                if (IsInRange(GetPositionX(), posGameobject.x, GetPositionY(), posGameobject.y, 200.0f))
                {
                    if (WillCollision(posBullet.x, posBullet.y, boundsBullet.height, boundsBullet.width, posGameobject.x, posGameobject.y, boundsGameobject.height, boundsGameobject.width))
                    {
                        Explode();
                        break;
                    }
                }
            }
        }

        if (GetPositionX() < 0 || GetPositionY() < 0)
            Explode();
    }

    Draw(&spriteBullet, true);
}

void Bullet::Draw(sf::Sprite* spriteBullet /* = NULL */, bool updatePos /* = false */)
{
    if (isRemoved)
        return;

    sf::Vector2f position = GetPosition();
    sf::Vector2f positionPlr = game->GetPlayer()->GetPosition();

    if (!IsInRange(position.x, positionPlr.x, position.y, positionPlr.y, 1000.0f))
        return;

    if (updatePos)
        spriteBullet->setPosition(position);

    if (GAME_STATE_PAUSED(game->GetGameState()))
        spriteBullet->setColor(sf::Color(255, 255, 255, 128));

    window->draw(*spriteBullet);
}

void Bullet::Explode()
{
    isRemoved = true;
}
