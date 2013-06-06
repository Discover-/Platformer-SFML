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
#include "enemy.h"
#include "bullet.h"
#include "shareddefines.h"
#include "level.h"

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

    sf::Texture imageMenuPlay;
    imageMenuPlay.loadFromFile("Graphics/Other/menu_button_play.png");
    sf::Sprite spriteMenuPlay(imageMenuPlay);
    spriteMenuPlay.setPosition(0.0f, 70.0f);

    sf::Texture imageMenuExit;
    imageMenuExit.loadFromFile("Graphics/Other/menu_button_exit.png");
    sf::Sprite spriteMenuExit(imageMenuExit);
    spriteMenuExit.setPosition(0.0f, 220.0f);

    sf::Texture imageMenuInfo;
    imageMenuInfo.loadFromFile("Graphics/Other/menu_button_info.png");
    sf::Sprite spriteMenuInfo(imageMenuInfo);
    spriteMenuInfo.setPosition(0.0f, 370.0f);

    sf::Texture imageMenuPlayLightup;
    imageMenuPlayLightup.loadFromFile("Graphics/Other/menu_button_play_lightup.png");
    sf::Sprite spriteMenuPlayLightup(imageMenuPlayLightup);
    spriteMenuPlayLightup.setPosition(0.0f, 70.0f);

    sf::Texture imageMenuExitLightup;
    imageMenuExitLightup.loadFromFile("Graphics/Other/menu_button_exit_lightup.png");
    sf::Sprite spriteMenuExitLightup(imageMenuExitLightup);
    spriteMenuExitLightup.setPosition(0.0f, 220.0f);

    sf::Texture imageMenuInfoLightup;
    imageMenuInfoLightup.loadFromFile("Graphics/Other/menu_button_info_lightup.png");
    sf::Sprite spriteMenuInfoLightup(imageMenuInfoLightup);
    spriteMenuInfoLightup.setPosition(0.0f, 370.0f);

    sf::Texture imageCharacter;
    imageCharacter.loadFromFile("Graphics/Characters/Tux/tux_from_linux-00-01.png");
    sf::Sprite spriteCharacter(imageCharacter);
    player = new Player(this, &window, 500, 70, spriteCharacter, TYPEID_PLAYER);

    sf::Texture imageEnemy;
    imageEnemy.loadFromFile("Graphics/Characters/Kit/frame_0.png");
    sf::Sprite spriteEnemy(imageEnemy);
    Enemy* enemy = new Enemy(this, &window, 50, 70, 400, 70, spriteEnemy, TYPEID_ENEMY);

    sf::Font font;
    font.loadFromFile("Fonts/arial.ttf");

    window.setFramerateLimit(30);

    sf::View view(window.getDefaultView());
    sf::Clock clock;
    sf::Clock fpsClock;

    Level* level = new Level(this);

    while (window.isOpen())
    {
        sf::Event _event;

        while (window.pollEvent(_event))
        {
            if (_event.type == sf::Event::Closed)
                window.close();

            if (_event.type == sf::Event::LostFocus && gameState == STATE_PLAYING)
            {
                window.setMouseCursorVisible(true);
                gameState = STATE_PAUSED_FOCUS;
            }

            if (_event.type == sf::Event::GainedFocus && gameState == STATE_PAUSED_FOCUS)
            {
                window.setMouseCursorVisible(false);
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
                sf::Sprite spritePlay = spriteMenuPlay;
                sf::Sprite spriteExit = spriteMenuExit;
                sf::Sprite spriteInfo = spriteMenuInfo;
                sf::Sprite* selection = NULL;

                float mouseX = float(sf::Mouse::getPosition(window).x);
                float mouseY = float(sf::Mouse::getPosition(window).y);

                if (WillCollision(mouseX, mouseY, 10.0f, 10.0f, spriteMenuPlay.getPosition().x, spriteMenuPlay.getPosition().y, spriteMenuPlay.getLocalBounds().height, spriteMenuPlay.getLocalBounds().width))
                {
                    spritePlay = spriteMenuPlayLightup;
                    selection = &spritePlay;
                }
                else if (WillCollision(mouseX, mouseY, 10.0f, 10.0f, spriteMenuExit.getPosition().x, spriteMenuExit.getPosition().y, spriteMenuExit.getLocalBounds().height, spriteMenuExit.getLocalBounds().width))
                {
                    spriteExit = spriteMenuExitLightup;
                    selection = &spriteExit;
                }
                else if (WillCollision(mouseX, mouseY, 10.0f, 10.0f, spriteMenuInfo.getPosition().x, spriteMenuInfo.getPosition().y, spriteMenuInfo.getLocalBounds().height, spriteMenuInfo.getLocalBounds().width))
                {
                    spriteInfo = spriteMenuInfoLightup;
                    selection = &spriteInfo;
                }

                window.draw(spritePlay);
                window.draw(spriteExit);
                window.draw(spriteInfo);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
                {
                    gameState = STATE_PLAYING;
                    level->LoadMap("Levels/level1.txt");
                    window.setMouseCursorVisible(false);
                    break;
                }

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    if (selection == &spritePlay)
                    {
                        gameState = STATE_PLAYING;
                        level->LoadMap("Levels/level1.txt");
                        window.setMouseCursorVisible(false);
                    }
                    else if (selection == &spriteExit)
                        window.close();
                    //else if (selection == &spriteInfo)
                    // NYI
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    window.close();

                break;
            }
            case STATE_PLAYING:
            {
                level->DrawMap(window);
                player->Update();
                enemy->Update();

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
                    window.setMouseCursorVisible(true);
                    gameState = STATE_PAUSED;
                }

                break;
            }
            case STATE_PAUSED:
            case STATE_PAUSED_FOCUS:
            {
                level->DrawMap(window);
                player->Update();
                enemy->Update();
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
    //enemy->HandleTimers(diff_time);
}
