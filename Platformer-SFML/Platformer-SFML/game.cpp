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
#include "Windows.h"
#include "game.h"
#include "player.h"
#include "bullet.h"
#include "shareddefines.h"
#include "level.h"
#include "menu.h"

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

    sf::Texture imageCharacter;
    imageCharacter.loadFromFile("Graphics/Characters/Tux/tux_from_linux-00-01.png");
    sf::Sprite spriteCharacter(imageCharacter);
    player = new Player(this, &window, 500, 70, spriteCharacter, TYPEID_PLAYER);

    sf::Texture imageEnemy;
    imageEnemy.loadFromFile("Graphics/Characters/Kit/frame_0.png");
    sf::Sprite spriteEnemy(imageEnemy);
    Enemy* enemy1 = new Enemy(this, &window, 166, 135, 400, 70, spriteEnemy, TYPEID_ENEMY);
    Enemy* enemy2 = new Enemy(this, &window, 514, 110, 710, 70, spriteEnemy, TYPEID_ENEMY);
    Enemy* enemy3 = new Enemy(this, &window, 950, 330, 1300, 330, spriteEnemy, TYPEID_ENEMY);
    allEnemies.push_back(enemy1);
    allEnemies.push_back(enemy2);
    allEnemies.push_back(enemy3);

    sf::Font font;
    font.loadFromFile("Fonts/arial.ttf");

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
        }

        HandleTimers(clock.restart().asMilliseconds());
        fpsClock.restart();
        window.clear();

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
                {
                    if (!(*itr)->IsRemoved())
                    {
                        (*itr)->Update();

                        //sf::Texture imageBullet;
                        //imageBullet.loadFromFile("Graphics/Other/menu_button_play.png");
                        //sf::Sprite spriteBullet;
                        //spriteBullet.setTexture(imageBullet);
                        //spriteBullet.setPosition((*itr)->GetPosXY());
                        //(*itr)->SetSprite(spriteBullet);
                        //window.draw((*itr)->GetSpriteBullet());
                    }
                }

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
                
                for (std::vector<Enemy*>::iterator itr = allEnemies.begin(); itr != allEnemies.end(); ++itr)
                    (*itr)->Update();

                break;
            }
            default:
                std::cout << "Unknown gamestate " << gameState << std::endl;
                break;
        }

        float fps = 1 / fpsClock.getElapsedTime().asSeconds();

        sf::Text text("FPS: " + std::to_string(long double(int(fps))), font, 15);
        text.setColor(GAME_STATE_DRAW_GAME(gameState) ? sf::Color::Black : sf::Color::White);
        text.setPosition(0.0f, 15.0f);
        window.draw(text);

        if (fps > 30)
        {
            sf::Text text2("Actual FPS: 30", font, 15);
            text2.setColor(GAME_STATE_DRAW_GAME(gameState) ? sf::Color::Black : sf::Color::White);
            text2.setPosition(0.0f, 0.0f);
            window.draw(text2);
        }

        window.display();
    }

    return 0;
}

void Game::HandleTimers(sf::Int32 diff_time)
{
    if (gameState != STATE_PLAYING)
        return;

    player->HandleTimers(diff_time);

    //for (std::vector<Enemy*>::iterator itr = allEnemies.begin(); itr != allEnemies.end(); ++itr)
        //(*itr)->HandleTimers(diff_time);
}

void Game::StartActualGame(sf::RenderWindow &window)
{
    gameState = STATE_PLAYING;
    currLevel->LoadMap("Levels/level1.txt");
    //window.setMouseCursorVisible(false);
}

