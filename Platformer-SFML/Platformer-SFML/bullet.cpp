#include "bullet.h"
#include "player.h"
#include "collision.h"
#include "game.h"
#include "shareddefines.h"

Bullet::Bullet(Game* _game, sf::RenderWindow* _window, float _x, float _y, sf::Texture _imageBullet, float _velocity /* = 5 */)
{
    if (!_game)
        return;

    game = _game;
    velocity = _velocity;
    isRemoved = false;
    posX = _x;
    posY = _y;
    window = _window;
    imageBullet = _imageBullet;
}

Bullet::~Bullet()
{

}

void Bullet::Update()
{
    Player* player = game->GetPlayer();

    if (isRemoved || !game || !game->IsRunning() || !player)
        return;

    bool isPaused = GAME_STATE_PAUSED_DRAWING(game->GetGameState());

    if (!isPaused)
        posX += velocity;

    sf::Sprite spriteBullet(imageBullet);
    spriteBullet.setPosition(posX, posY);

    if (!isPaused)
    {
        sf::Sprite spriteChar = player->GetSpriteBody();
        sf::Vector2f posBullet = spriteBullet.getPosition();
        sf::Vector2f posChar = spriteChar.getPosition();
        sf::FloatRect boundsBullet = spriteBullet.getGlobalBounds();
        sf::FloatRect boundsChar = spriteChar.getGlobalBounds();

        if (WillCollision(posX, posY, boundsBullet.height, boundsBullet.width, posChar.x, posChar.y, boundsChar.height, boundsChar.width))
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

                if (IsInRange(posX, enemyX, posY, enemyY, 200.0f))
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
    }

    Draw(&spriteBullet, true);
}

void Bullet::Draw(sf::Sprite* spriteBullet /* = NULL */, bool updatePos /* = false */)
{
    if (isRemoved)
        return;

    if (updatePos)
        spriteBullet->setPosition(posX, posY);

    if (GAME_STATE_PAUSED_DRAWING(game->GetGameState()))
        spriteBullet->setColor(sf::Color(255, 255, 255, 128));

    window->draw(*spriteBullet);
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
}
