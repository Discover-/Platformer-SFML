#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <time.h>
#include <unordered_map>
#include <set>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "Windows.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "shareddefines.h"

Game::Game()
{
    isRunning = true;
}

Game::~Game()
{

}

int Game::Update()
{
    isRunning = true;
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Platformer C++ SFML");

    sf::Texture imageCharacter;
    imageCharacter.loadFromFile("tux_frame_0.png");
    sf::Sprite spriteCharacter(imageCharacter);
    player = new Player(this, &window, 500, 70, spriteCharacter, TYPEID_PLAYER);

    sf::Texture imageEnemy;
    imageEnemy.loadFromFile("kit_frame_0.png");
    sf::Sprite spriteEnemy(imageEnemy);
    Enemy* enemy = new Enemy(this, &window, 50, 70, 400, 70, spriteEnemy, TYPEID_ENEMY);

    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Texture imageDirt[2];
    sf::Texture imageGrass[2];
    sf::Texture imageGround[2];
    sf::Texture imageSky[4];
    sf::Sprite spriteDirt[15][60];
    sf::Sprite spriteGrass[12][60];
    sf::Sprite spriteGround[12][60];
    sf::Sprite spriteSky[12][60];

    for (int i = 0; i < 2; ++i)
    {
        std::string numberInStr = std::to_string(long double(i));
        imageDirt[i].loadFromFile("dirt_" + numberInStr + ".png");
        imageGrass[i].loadFromFile("grass_" + numberInStr + ".png");
        imageGround[i].loadFromFile("ground_" + numberInStr + ".png");
    }

    for (int i = 0; i < 4; ++i)
        imageSky[i].loadFromFile("sky_" + std::to_string(long double(i)) + ".png");

    for (int i = 7; i < 9; ++i)
        for (int j = 0; j < 60; ++j)
            spriteGrass[i][j].setTexture(imageGrass[urand(0, 1)]);

    for (int i = 9; i < 12; ++i)
        for (int j = 0; j < 60; ++j)
            spriteGround[i][j].setTexture(imageGround[urand(0, 1)]);

    for (int i = 12; i < 15; ++i)
        for (int j = 0; j < 60; ++j)
            spriteDirt[i][j].setTexture(imageDirt[urand(0, 1)]);

    for (int i = 0; i < 12; ++i)
        for (int j = 0; j < 60; ++j)
            spriteSky[i][j].setTexture(imageSky[(i > 7 || urand(0, 20) < 16) ? 3 : urand(0, 2)]);

    float boxX = 0.0f, boxY = 0.0f;

    //! Filling up skybox
    for (int i = 0; i < 12; ++i)
    {
        for (int j = 0; j < 60; ++j)
        {
            spriteSky[i][j].setPosition(boxX, boxY);
            boxX += 50.0f;
        }

        boxX = 0.0f;
        boxY += 50.0f;
    }

    //! Filling up grass
    boxX = 0.0f;
    boxY = 250.0f;

    for (int i = 7; i < 9; ++i)
    {
        for (int j = 0; j < 60; ++j)
        {
            spriteGrass[i][j].setPosition(boxX, boxY);
            boxX += 50.0f;
        }

        boxX = 0.0f;
        boxY += 50.0f;
    }

    //! Filling up ground
    boxX = 0.0f;
    boxY = 350.0f;

    for (int i = 9; i < 12; ++i)
    {
        for (int j = 0; j < 60; ++j)
        {
            spriteGround[i][j].setPosition(boxX, boxY);
            boxX += 50.0f;
        }

        boxX = 0.0f;
        boxY += 50.0f;
    }

    //! Filling up dirt
    boxX = 0.0f;
    boxY = 500.0f;

    for (int i = 12; i < 15; ++i)
    {
        for (int j = 0; j < 60; ++j)
        {
            spriteDirt[i][j].setPosition(boxX, boxY);
            boxX += 50.0f;
        }

        boxX = 0.0f;
        boxY += 50.0f;
    }

    window.setFramerateLimit(30);
    window.setMouseCursorVisible(false);

    //! Sky blocks are not collidable.
    for (int i = 0; i < 12; ++i)
        for (int j = 0; j < 60; ++j)
            gameObjects.push_back(spriteSky[i][j]);

    for (int i = 9; i < 12; ++i)
    {
        for (int j = 0; j < 60; ++j)
        {
            gameObjects.push_back(spriteGround[i][j]);
            gameObjectsCollidable.push_back(spriteGround[i][j]);
        }
    }

    for (int i = 7; i < 9; ++i)
    {
        for (int j = 0; j < 60; ++j)
        {
            gameObjects.push_back(spriteGrass[i][j]);
            gameObjectsCollidable.push_back(spriteGrass[i][j]);
        }
    }

    for (int i = 12; i < 15; ++i)
    {
        for (int j = 0; j < 60; ++j)
        {
            gameObjects.push_back(spriteDirt[i][j]);
            gameObjectsCollidable.push_back(spriteDirt[i][j]);
        }
    }

    sf::View view(window.getDefaultView());
    sf::Clock clock;
    sf::Clock fpsClock;

    while (window.isOpen())
    {
        HandleTimers(clock.restart().asMilliseconds());
        fpsClock.restart();

        sf::Event _event;

        while (window.pollEvent(_event))
        {
            if (_event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (int i = 0; i < 12; ++i)
            for (int j = 0; j < 60; ++j)
                window.draw(spriteSky[i][j]);

        for (int i = 9; i < 12; ++i)
            for (int j = 0; j < 60; ++j)
                window.draw(spriteGround[i][j]);

        for (int i = 7; i < 9; ++i)
            for (int j = 0; j < 60; ++j)
                window.draw(spriteGrass[i][j]);

        for (int i = 12; i < 15; ++i)
            for (int j = 0; j < 60; ++j)
                window.draw(spriteDirt[i][j]);

        float posX, posY;
        player->GetPosition(posX, posY);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            if (!player->CollidesWithGameobjects(player->GetPositionX() - 4.0f, player->GetPositionY() + 5.0f))
                player->SetPositionXY(posX -= player->GetMoveSpeed(), posY);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            if (!player->CollidesWithGameobjects(player->GetPositionX() + 4.0f, player->GetPositionY() + 5.0f))
                player->SetPositionXY(posX += player->GetMoveSpeed(), posY);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            if (!player->IsJumping() && !player->IsFalling())
                player->SetIsJumping(true);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            if (player->CanShoot())
                player->Shoot();

        if (player->GetPositionX() > window.getSize().x / 2.f)
            view.setCenter(player->GetPositionX(), view.getCenter().y);
        else
            view.setCenter(window.getSize().x / 2.f, view.getCenter().y);

        if (player->GetPositionY() > window.getSize().y / 2.f)
            view.setCenter(view.getCenter().x, player->GetPositionY());
        else
            view.setCenter(view.getCenter().x, window.getSize().y / 2.f);

        player->Update();
        enemy->Update();

        for (std::vector<Bullet*>::iterator itr = allBullets.begin(); itr != allBullets.end(); ++itr)
        {
            if (!(*itr)->IsRemoved())
            {
                (*itr)->Update();

                //sf::Texture imageBullet;
                //imageBullet.loadFromFile("bullet.png");
                //sf::Sprite spriteBullet;
                //spriteBullet.setTexture(imageBullet);
                //spriteBullet.setPosition((*itr)->GetPosXY());
                //(*itr)->SetSprite(spriteBullet);
                //window.draw((*itr)->GetSpriteBullet());
            }
        }

        window.setView(view);

        float fps = 1 / fpsClock.getElapsedTime().asSeconds();

        if (fps > 30)
        {
            sf::Text text("Actual FPS: 30", font, 15);
            text.setColor(sf::Color::Black);
            text.setPosition(400 + player->GetPositionX(), 0.0f);
            window.draw(text);
        }

        sf::Text text("FPS: " + std::to_string(long double(int(fps))), font, 15);
        text.setColor(sf::Color::Black);
        text.setPosition(436 + player->GetPositionX(), 15.0f);
        window.draw(text);
        window.display();
    }

    return 0;
}

void Game::HandleTimers(sf::Int32 diff_time)
{
    player->HandleTimers(diff_time);
}
