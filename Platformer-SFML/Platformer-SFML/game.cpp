#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <time.h>
#include <unordered_map>
#include <set>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "game.h"
#include "player.h"
#include "bullet.h"
#include "shareddefines.h"
#include "level.h"
#include "menu.h"
#include "enemy.h"
#include "movingtile.h"

Game::Game()
{
    isRunning = true;
    gameState = STATE_MENU;
}

Game::~Game()
{

}

int Game::Update()
{
    isRunning = true;
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Platformer C++ SFML");
    //window.setKeyRepeatEnabled(false);

    sf::Texture imageCharacter;
    std::vector<std::pair<int, sf::Texture>> spriteCharactersLeft;
    std::vector<std::pair<int, sf::Texture>> spriteCharactersRight;

    for (int j = 0; j < 2; ++j)
    {
        for (int i = 0; i < 10; ++i)
        {
            imageCharacter.loadFromFile("Graphics/Character/walk_" + std::to_string(long double(i)) + "_" + (j ? "l" : "r") + ".png");
            j ? spriteCharactersRight.push_back(std::make_pair(i, imageCharacter)) : spriteCharactersLeft.push_back(std::make_pair(i, imageCharacter));
        }
    }

    player = new Player(this, &window, 500, 70, spriteCharactersLeft, spriteCharactersRight, TYPEID_PLAYER, 5, 9, 30, false);

    sf::Texture imageEnemy;
    std::vector<std::pair<int, sf::Texture>> spriteEnemiesLeft;
    std::vector<std::pair<int, sf::Texture>> spriteEnemiesRight;

    for (int j = 0; j < 2; ++j)
    {
        for (int i = 0; i < 2; ++i)
        {
            imageEnemy.loadFromFile("Graphics/Enemies/fly_" + std::string(i ? "fly_" : "normal_") + (j ? "l" : "r") + ".png");
            j ? spriteEnemiesRight.push_back(std::make_pair(i, imageEnemy)) : spriteEnemiesLeft.push_back(std::make_pair(i, imageEnemy));
        }
    }

    Enemy* enemy1 = new Enemy(this, &window, 166.0f, 345.0f, 400.0f, 345.0f, spriteEnemiesLeft, spriteEnemiesRight, TYPEID_ENEMY, 3, 1, 80, true);
    Enemy* enemy2 = new Enemy(this, &window, 845.0f, 230.0f, 1250.0f, 230.0f, spriteEnemiesLeft, spriteEnemiesRight, TYPEID_ENEMY, 3, 1, 80, true);
    Enemy* enemy3 = new Enemy(this, &window, 235.0f, 89.0f, 620.0f, 89.0f, spriteEnemiesLeft, spriteEnemiesRight, TYPEID_ENEMY, 3, 1, 80, true);

    spriteEnemiesLeft.clear();
    spriteEnemiesRight.clear();

    for (int j = 0; j < 2; ++j)
    {
        for (int i = 0; i < 2; ++i)
        {
            imageEnemy.loadFromFile("Graphics/Enemies/slime_" + std::string(i ? "walk_" : "normal_") + (j ? "l" : "r") + ".png");
            j ? spriteEnemiesRight.push_back(std::make_pair(i, imageEnemy)) : spriteEnemiesLeft.push_back(std::make_pair(i, imageEnemy));
        }
    }

    Enemy* enemy4 = new Enemy(this, &window, 450.0f, 190.0f, 650.0f, 190.0f, spriteEnemiesLeft, spriteEnemiesRight, TYPEID_ENEMY, 3, 1, 200, false);

    allEnemies.push_back(enemy1);
    allEnemies.push_back(enemy2);
    allEnemies.push_back(enemy3);
    allEnemies.push_back(enemy4);

    sf::Texture textureTile;
    textureTile.loadFromFile("Graphics/Tiles/shroom.png");
    sf::Vector2f tileStart(700.0f, 250.0f);
    sf::Vector2f tileDesti(900.0f, 450.0f);
    MovingTile* movingTile1 = new MovingTile(this, &window, textureTile, 3, tileStart, tileDesti);

    sf::Vector2f tileStart2(900.0f, 50.0f);
    sf::Vector2f tileDesti2(1100.0f, 250.0f);
    MovingTile* movingTile2 = new MovingTile(this, &window, textureTile, 3, tileStart2, tileDesti2);
    movingTiles.push_back(movingTile1);
    movingTiles.push_back(movingTile2);
    gameObjectsCollidable.push_back(movingTile1->GetSprite());
    gameObjectsCollidable.push_back(movingTile2->GetSprite());

    sf::Font font;
    font.loadFromFile("Fonts/Market_Deco.ttf");

    window.setFramerateLimit(30);

    sf::View view(window.getDefaultView());
    sf::Clock clock;
    sf::Clock fpsClock;

    currLevel = new Level(this);
    Menu* menu = new Menu(this);
    menu->Load();

    while (window.isOpen())
    {
        sf::Event _event;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            std::cout << "Mouse X: " << sf::Mouse::getPosition(window).x << std::endl;
            std::cout << "Mouse Y: " << sf::Mouse::getPosition(window).y << std::endl;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
            currLevel->LoadMap("Levels/level1.txt");
        //! Open up new instance of the game.
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
            return Update();

        while (window.pollEvent(_event))
        {
            if (_event.type == sf::Event::Closed)
                window.close();

            if (_event.type == sf::Event::LostFocus && gameState == STATE_PLAYING)
            {
                //window.setMouseCursorVisible(true);
                gameState = STATE_PAUSED_FOCUS;
            }

            if (_event.type == sf::Event::GainedFocus && gameState == STATE_PAUSED_FOCUS)
            {
                //window.setMouseCursorVisible(false);
                gameState = STATE_PLAYING;
            }

            //if (_event.type == sf::Event::KeyReleased)
            //    if (_event.key.code)

        }

        HandleTimers(clock.restart().asMilliseconds());
        fpsClock.restart();

        sf::Color colorSky;
        colorSky.r = 136;
        colorSky.g = 247;
        colorSky.b = 255;
        window.clear(colorSky);

        for (std::vector<MovingTile*>::iterator itr = movingTiles.begin(); itr != movingTiles.end(); ++itr)
        {
            sf::FloatRect movingTileRect = (*itr)->GetSprite().getGlobalBounds();
            sf::FloatRect playerRect = player->GetSpriteBody().getGlobalBounds();

            if (WillCollision(player->GetPositionX(), player->GetPositionY(), playerRect.height, playerRect.width, (*itr)->GetPositionX(), (*itr)->GetPositionY(), movingTileRect.height, movingTileRect.width))
            {
                if (!(*itr)->HasPassenger(player))
                    (*itr)->AddPassenger(player);
            }
            else if ((*itr)->HasPassenger(player))
                (*itr)->RemovePassenger(player);
        }

        switch (gameState)
        {
            case STATE_MENU:
            {
                menu->Update(window);
                break;
            }
            case STATE_PLAYING:
            {
                currLevel->DrawMap(window);
                player->Update();
                player->DrawHearts(window, view);

                for (std::vector<MovingTile*>::iterator itr = movingTiles.begin(); itr != movingTiles.end(); ++itr)
                    (*itr)->Update();

                for (std::vector<Enemy*>::iterator itr = allEnemies.begin(); itr != allEnemies.end(); ++itr)
                    (*itr)->Update();

                if (player->GetPositionX() > window.getSize().x / 2.f)
                    view.setCenter(player->GetPositionX(), view.getCenter().y);
                else
                    view.setCenter(window.getSize().x / 2.f, view.getCenter().y);

                if (player->GetPositionY() > window.getSize().y / 2.f)
                    view.setCenter(view.getCenter().x, player->GetPositionY());
                else
                    view.setCenter(view.getCenter().x, window.getSize().y / 2.f);

                window.setView(view);

                for (std::vector<Bullet*>::iterator itr = allBullets.begin(); itr != allBullets.end(); ++itr)
                    if (!(*itr)->IsRemoved())
                        (*itr)->Update();

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    //window.setMouseCursorVisible(true);
                    gameState = STATE_PAUSED;
                }

                break;
            }
            case STATE_PAUSED:
            case STATE_PAUSED_FOCUS:
            {
                currLevel->DrawMap(window);
                player->Update();
                player->DrawHearts(window, view);

                for (std::vector<MovingTile*>::iterator itr = movingTiles.begin(); itr != movingTiles.end(); ++itr)
                    (*itr)->Update();

                for (std::vector<Enemy*>::iterator itr = allEnemies.begin(); itr != allEnemies.end(); ++itr)
                    (*itr)->Update();

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    //window.setMouseCursorVisible(false);
                    gameState = STATE_PLAYING;
                }
                
                sf::Text textPaused("Paused", font, 80);
                textPaused.setColor(sf::Color::White);
                textPaused.setPosition(view.getCenter().x, view.getCenter().y);
                window.draw(textPaused);
                break;
            }
            default:
                std::cout << "Unknown gamestate " << gameState << std::endl;
                break;
        }

        if (gameState != STATE_MENU)
        {
            sf::Text text("Position X: " + std::to_string(long double(int(player->GetPositionX()))) + "\nPosition Y: " + std::to_string(long double(int(player->GetPositionY()))), font, 15);
            text.setColor(GAME_STATE_DRAW_GAME(gameState) ? sf::Color::Black : sf::Color::White);
            text.setPosition(view.getCenter().x + 375.0f, view.getCenter().y - 300.0f);
            window.draw(text);
        }

        float fps = 1 / fpsClock.getElapsedTime().asSeconds();

        sf::Text text2("FPS: " + std::to_string(long double(int(fps))), font, 15);
        text2.setColor(GAME_STATE_DRAW_GAME(gameState) ? sf::Color::Black : sf::Color::White);
        text2.setPosition(view.getCenter().x + 375.0f, view.getCenter().y - 265.0f);
        window.draw(text2);

        window.display();
    }

    return 0;
}

void Game::HandleTimers(sf::Int32 diff_time)
{
    //if (gameState != STATE_PLAYING)
    //    return;

    player->HandleTimers(diff_time);

    for (std::vector<Enemy*>::iterator itr = allEnemies.begin(); itr != allEnemies.end(); ++itr)
        if (!(*itr)->IsDead())
            (*itr)->HandleTimers(diff_time);
}

void Game::StartActualGame(sf::RenderWindow &window)
{
    gameState = STATE_PLAYING;
    currLevel->LoadMap("Levels/level1.txt");
    //window.setMouseCursorVisible(false);
}
