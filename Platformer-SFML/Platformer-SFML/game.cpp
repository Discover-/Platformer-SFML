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
#include "bouncetile.h"
#include "coin.h"

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
            imageCharacter.loadFromFile("Graphics/Character/walk_" + std::to_string(static_cast<long long>(i)) + "_" + (j ? "l" : "r") + ".png");
            j ? spriteCharactersRight.push_back(std::make_pair(i, imageCharacter)) : spriteCharactersLeft.push_back(std::make_pair(i, imageCharacter));
        }
    }

    player = new Player(this, &window, sf::Vector2f(165.0f, 135.0f), spriteCharactersLeft, spriteCharactersRight, TYPEID_PLAYER, 5, 9, 30, false);

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

    Enemy* enemy1 = new Enemy(this, &window, sf::Vector2f(166.0f, 345.0f), sf::Vector2f(400.0f, 345.0f), spriteEnemiesLeft, spriteEnemiesRight, TYPEID_ENEMY, 3, 1, 80, true);
    Enemy* enemy2 = new Enemy(this, &window, sf::Vector2f(845.0f, 230.0f), sf::Vector2f(1250.0f, 230.0f), spriteEnemiesLeft, spriteEnemiesRight, TYPEID_ENEMY, 3, 1, 80, true);
    Enemy* enemy3 = new Enemy(this, &window, sf::Vector2f(235.0f, 89.0f), sf::Vector2f(620.0f, 89.0f), spriteEnemiesLeft, spriteEnemiesRight, TYPEID_ENEMY, 3, 1, 80, true);

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

    Enemy* enemy4 = new Enemy(this, &window, sf::Vector2f(450.0f, 190.0f), sf::Vector2f(650.0f, 190.0f), spriteEnemiesLeft, spriteEnemiesRight, TYPEID_ENEMY, 3, 1, 200, false);

    allEnemies.push_back(enemy1);
    allEnemies.push_back(enemy2);
    allEnemies.push_back(enemy3);
    allEnemies.push_back(enemy4);

    sf::Font font;
    font.loadFromFile("Fonts/Market_Deco.ttf");

    window.setFramerateLimit(30);

    sf::View view(window.getDefaultView());
    sf::Clock clock, fpsClock;

    currLevel = new Level(this);
    Menu* menu = new Menu(this);
    menu->Load();

    while (window.isOpen())
    {
        sf::Event _event;

        while (window.pollEvent(_event))
        {
            if (_event.type == sf::Event::Closed)
                window.close();

            if (_event.type == sf::Event::MouseButtonReleased && _event.mouseButton.button == sf::Mouse::Left && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                std::cout << "Mouse X: " << sf::Mouse::getPosition(window).x << std::endl;
                std::cout << "Mouse Y: " << sf::Mouse::getPosition(window).y << std::endl;
            }

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

            if (_event.type == sf::Event::KeyReleased)
            {
                switch (_event.key.code)
                {
                    //! Reload map
                    case sf::Keyboard::F1:
                        currLevel->LoadMap("Levels/level1.txt", window);
                        break;
                    //! Pause or un-pause game based on current gamestate.
                    case sf::Keyboard::Escape:
                        if (gameState != STATE_MENU)
                        {
                            gameState = gameState == STATE_PLAYING ? STATE_PAUSED : STATE_PLAYING;
                            //window.setMouseCursorVisible(gameState == STATE_PLAYING);
                        }
                        else
                            window.close();
                        break;
                    //! Move menu option selection up
                    case sf::Keyboard::Up:
                        if (gameState == STATE_MENU)
                            menu->SetSelectedOption(menu->GetSelectedOption() + 1);
                        break;
                    //! Move menu option selection down
                    case sf::Keyboard::Down:
                        if (gameState == STATE_MENU)
                            menu->SetSelectedOption(menu->GetSelectedOption() - 1);
                        break;
                    //! Select menu option
                    case sf::Keyboard::Return:
                    case sf::Mouse::Left:
                        if (gameState == STATE_MENU)
                            menu->PressedEnterOrMouse(window);
                        break;
                    case sf::Keyboard::Space:
                        if (gameState == STATE_PLAYING && player->CanShoot())
                            player->Shoot();
                        break;
                }
            }

            if (_event.type == sf::Event::MouseButtonPressed)
            {
                switch (_event.mouseButton.button)
                {
                    //! Select menu option
                    case sf::Mouse::Left:
                        if (gameState == STATE_MENU)
                            menu->PressedEnterOrMouse(window);
                        break;
                    case sf::Mouse::Right:
                        break;
                }
            }

        }

        HandleTimers(clock.restart().asMilliseconds());
        fpsClock.restart();

        sf::Color colorSky;
        colorSky.r = 136;
        colorSky.g = 247;
        colorSky.b = 255;
        window.clear(gameState == STATE_MENU ? sf::Color() : colorSky);

        for (std::vector<Tile*>::iterator itr = allTiles.begin(); itr != allTiles.end(); ++itr)
        {
            sf::FloatRect tileRect = (*itr)->GetSpriteTile().getGlobalBounds();
            sf::FloatRect playerRect = player->GetSpriteBody().getGlobalBounds();

            if (WillCollision((*itr)->GetPositionX(), (*itr)->GetPositionY(), tileRect.height, tileRect.width, player->GetPositionX(), player->GetPositionY(), playerRect.height, playerRect.width))
            {
                if ((*itr)->GetTypeId() == TYPEID_MOVING_TILE)
                {
                    if (!((MovingTile*)(*itr))->HasPassenger(player))
                        ((MovingTile*)(*itr))->AddPassenger(player);
                }
                else if ((*itr)->GetTypeId() == TYPEID_BOUNCE_TILE)
                {
                    if (!((BounceTile*)(*itr))->IsUsed())
                    {
                        ((BounceTile*)(*itr))->SetIsUsed(true);
                        player->Jump(30);
                    }
                }
            }
            else
            {
                if ((*itr)->GetTypeId() == TYPEID_MOVING_TILE)
                {
                    if (((MovingTile*)(*itr))->HasPassenger(player))
                        ((MovingTile*)(*itr))->RemovePassenger(player);
                }
                else if ((*itr)->GetTypeId() == TYPEID_BOUNCE_TILE)
                {

                }
            }
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

                for (std::vector<Tile*>::iterator itr = allTiles.begin(); itr != allTiles.end(); ++itr)
                    (*itr)->Update();

                for (std::vector<Coin*>::iterator itr = allCoins.begin(); itr != allCoins.end(); ++itr)
                    if (!(*itr)->IsTaken())
                        (*itr)->Update();

                for (std::vector<Enemy*>::iterator itr = allEnemies.begin(); itr != allEnemies.end(); ++itr)
                    (*itr)->Update();

                for (std::vector<Bullet*>::iterator itr = allBullets.begin(); itr != allBullets.end(); ++itr)
                    if (!(*itr)->IsRemoved())
                        (*itr)->Update();

                player->Update();
                player->DrawAccessoires(window, view);

                if (player->GetPositionX() > window.getSize().x / 2.f)
                    view.setCenter(player->GetPositionX(), view.getCenter().y);
                else
                    view.setCenter(window.getSize().x / 2.f, view.getCenter().y);

                if (player->GetPositionY() > window.getSize().y / 2.f)
                    view.setCenter(view.getCenter().x, player->GetPositionY());
                else
                    view.setCenter(view.getCenter().x, window.getSize().y / 2.f);

                window.setView(view);

                break;
            }
            case STATE_PAUSED:
            case STATE_PAUSED_FOCUS:
            {
                currLevel->DrawMap(window);
                player->Update();
                player->DrawAccessoires(window, view);

                for (std::vector<Tile*>::iterator itr = allTiles.begin(); itr != allTiles.end(); ++itr)
                    (*itr)->Update();

                for (std::vector<Coin*>::iterator itr = allCoins.begin(); itr != allCoins.end(); ++itr)
                    if (!(*itr)->IsTaken())
                        (*itr)->Update();

                for (std::vector<Enemy*>::iterator itr = allEnemies.begin(); itr != allEnemies.end(); ++itr)
                    (*itr)->Update();

                for (std::vector<Bullet*>::iterator itr = allBullets.begin(); itr != allBullets.end(); ++itr)
                    if (!(*itr)->IsRemoved())
                        (*itr)->Update();
                
                sf::Text textPaused("Paused", font, 80);
                textPaused.setColor(sf::Color::White);
                textPaused.setPosition(view.getCenter().x - (textPaused.getLocalBounds().width / 2.0f), view.getCenter().y - (textPaused.getLocalBounds().height / 2.0f));
                window.draw(textPaused);
                break;
            }
            default:
                std::cout << "Unknown gamestate " << gameState << std::endl;
                break;
        }

        if (gameState != STATE_MENU)
        {
            sf::Text text("Position X: " + std::to_string(static_cast<long long>(player->GetPositionX())) + "\nPosition Y: " + std::to_string(static_cast<long long>(player->GetPositionY())), font, 15);
            text.setColor(GAME_STATE_DRAW_GAME(gameState) ? sf::Color::Black : sf::Color::White);
            text.setPosition(view.getCenter().x + 375.0f, view.getCenter().y - 300.0f);
            window.draw(text);

            float fps = 1 / fpsClock.getElapsedTime().asSeconds();
            sf::Text text2("FPS: " + std::to_string(static_cast<long long>(fps)), font, 15);
            text2.setColor(GAME_STATE_DRAW_GAME(gameState) ? sf::Color::Black : sf::Color::White);
            text2.setPosition(view.getCenter().x + 375.0f, view.getCenter().y - 265.0f);
            window.draw(text2);
        }

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

    for (std::vector<Tile*>::iterator itr = allTiles.begin(); itr != allTiles.end(); ++itr)
        if (!(*itr)->IsRemoved())
            (*itr)->HandleTimers(diff_time);
}

void Game::StartActualGame(sf::RenderWindow &window)
{
    gameState = STATE_PLAYING;
    currLevel->LoadMap("Levels/level1.txt", window);
    //window.setMouseCursorVisible(false);
}

bool Game::IsQuickSandArea(float x, float y, float h, float w)
{
    for (std::vector<sf::Sprite>::iterator itr = quickSandGameobjects.begin(); itr != quickSandGameobjects.end(); ++itr)
    {
        sf::Vector2f quickSandPos = (*itr).getPosition();
        sf::FloatRect quickSandRect = (*itr).getGlobalBounds();

        //! We add 5 pixels to the height so it properly checks for gameobjects we're moving on.
        if (WillCollision(x, y, h + 5, w, quickSandPos.x, quickSandPos.y, quickSandRect.height, quickSandRect.width))
            return true;
    }

    return false;
}

bool Game::IsInWaterArea(float x, float y, float h, float w)
{
    for (std::vector<sf::Sprite>::iterator itr = waterGameobjects.begin(); itr != waterGameobjects.end(); ++itr)
    {
        sf::Vector2f waterPos = (*itr).getPosition();
        sf::FloatRect waterRect = (*itr).getGlobalBounds();

        //! We add 5 pixels to the height so it properly checks for gameobjects we're moving on.
        if (WillCollision(x, y, h + 5, w, waterPos.x, waterPos.y, waterRect.height, waterRect.width))
            return true;
    }

    return false;
}

bool Game::IsInLavaArea(float x, float y, float h, float w)
{
    for (std::vector<sf::Sprite>::iterator itr = lavaGameobjects.begin(); itr != lavaGameobjects.end(); ++itr)
    {
        sf::Vector2f lavaPos = (*itr).getPosition();
        sf::FloatRect lavaRect = (*itr).getGlobalBounds();

        if (WillCollision(x, y, h, w, lavaPos.x, lavaPos.y, lavaRect.height, lavaRect.width))
            return true;
    }

    return false;
}
