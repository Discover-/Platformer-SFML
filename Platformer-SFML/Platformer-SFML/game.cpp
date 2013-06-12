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
#include "bonustile.h"
#include "coin.h"
#include "menuplayer.h"

Game::Game()
{
    isRunning = true;
    showDebugInfo = false;
    gameState = STATE_MAIN_MENU;
}

Game::~Game()
{

}

int Game::Update()
{
    sf::Clock clockStart;
    clockStart.restart();

    sf::RenderWindow window(sf::VideoMode(1000, 600), "Platformer C++ SFML");
    window.setFramerateLimit(30);

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

    player = new Player(this, &window, sf::Vector2f(165.0f, 85.0f), spriteCharactersLeft, spriteCharactersRight, 5, 9, 30, false);
    menuPlayer = new MenuPlayer(this, &window, sf::Vector2f(165.0f, 285.0f), spriteCharactersLeft, 9, 30);

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

    Enemy* enemy1 = new Enemy(this, &window, sf::Vector2f(166.0f, 295.0f), sf::Vector2f(400.0f, 295.0f), spriteEnemiesLeft, spriteEnemiesRight, 3, 1, 80, true);
    Enemy* enemy2 = new Enemy(this, &window, sf::Vector2f(845.0f, 180.0f), sf::Vector2f(1250.0f, 180.0f), spriteEnemiesLeft, spriteEnemiesRight, 3, 1, 80, true);
    Enemy* enemy3 = new Enemy(this, &window, sf::Vector2f(235.0f, 39.0f), sf::Vector2f(620.0f, 39.0f), spriteEnemiesLeft, spriteEnemiesRight, 3, 1, 80, true);

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

    Enemy* enemy4 = new Enemy(this, &window, sf::Vector2f(450.0f, 140.0f), sf::Vector2f(650.0f, 140.0f), spriteEnemiesLeft, spriteEnemiesRight, 3, 1, 200, false);

    allEnemies.push_back(enemy1);
    allEnemies.push_back(enemy2);
    allEnemies.push_back(enemy3);
    allEnemies.push_back(enemy4);

    for (std::vector<Enemy*>::iterator itr = allEnemies.begin(); itr != allEnemies.end(); ++itr)
        allUnits.push_back(*itr);

    allUnits.push_back(player);
    allUnits.push_back(menuPlayer);

    sf::Font font;
    font.loadFromFile("Fonts/Market_Deco.ttf");

    sf::View view(window.getDefaultView());
    sf::Clock clock, fpsClock;

    currLevel = new Level(this, window);
    Menu* menu = new Menu(this);
    menu->Load();

    std::cout << "Time in milliseconds taken to load everything before entering while-loop: " << clockStart.restart().asMilliseconds() << std::endl;

    while (window.isOpen())
    {
        sf::Event _event;

        bool shiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

        while (window.pollEvent(_event))
        {
            if (_event.type == sf::Event::Closed)
                window.close();

            if (_event.type == sf::Event::MouseButtonReleased && _event.mouseButton.button == sf::Mouse::Left && shiftPressed)
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
                    {
                        sf::Clock _clock; _clock.restart();
                        std::string levelFilename = "Levels/level";
                        levelFilename += shiftPressed ? "_menu.txt" : "1.txt";
                        currLevel->LoadMap(levelFilename.c_str(), window);
                        break;
                    }
                    //! Back to menu
                    case sf::Keyboard::F2:
                    {
                        gameState = STATE_MAIN_MENU;
                        currLevel->LoadMap("Levels/level_menu.txt", window);
                        menuPlayer->SetPosition(1200.0f, 285.0f);
                        break;
                    }
                    //! Turn on/off debug information
                    case sf::Keyboard::F3:
                    {
                        showDebugInfo = !showDebugInfo;
                        break;
                    }
                    //! Pause or un-pause game based on current gamestate.
                    case sf::Keyboard::Escape:
                        if (gameState != STATE_MAIN_MENU)
                        {
                            gameState = gameState == STATE_PLAYING ? STATE_PAUSED : STATE_PLAYING;
                            //window.setMouseCursorVisible(gameState == STATE_PLAYING);
                        }
                        else
                            window.close();
                        break;
                    //! Move menu option selection up
                    case sf::Keyboard::Up:
                        if (gameState == STATE_MAIN_MENU)
                        {
                            int selection = menu->GetSelectedOption();
                            menu->SetSelectedOption(selection == 1 ? 4 : selection - 1);
                        }
                        break;
                    //! Move menu option selection down
                    case sf::Keyboard::Down:
                        if (gameState == STATE_MAIN_MENU)
                        {
                            int selection = menu->GetSelectedOption();
                            menu->SetSelectedOption(selection == 4 ? 1 : selection + 1);
                        }
                        break;
                    //! Select menu option
                    case sf::Keyboard::Return:
                    case sf::Mouse::Left:
                        if (gameState == STATE_MAIN_MENU)
                            menu->PressedEnterOrMouse(window);
                        break;
                    case sf::Keyboard::Space:
                        if (gameState == STATE_PLAYING && player->CanShoot())
                            player->Shoot();
                        break;
                    case sf::Keyboard::T:
                        if (shiftPressed)
                            for (std::vector<Enemy*>::iterator itr = allEnemies.begin(); itr != allEnemies.end(); ++itr)
                                if (!(*itr)->IsDead())
                                    (*itr)->JustDied();
                        break;
                    default:
                        break;
                }
            }
            else if (_event.type == sf::Event::MouseWheelMoved)
            {
                if (gameState == STATE_MAIN_MENU)
                {
                    int selection = menu->GetSelectedOption();
                    int ticks = _event.mouseWheel.delta;
                    menu->SetSelectedOption(ticks > 0 ? selection >= 4 ? 1 : selection + ticks : selection <= 1 ? 4 : selection + ticks);
                }
            }
            else if (_event.type == sf::Event::MouseButtonPressed)
            {
                switch (_event.mouseButton.button)
                {
                    //! Select menu option
                    case sf::Mouse::Left:
                        if (gameState == STATE_MAIN_MENU)
                            menu->PressedEnterOrMouse(window);
                        break;
                    case sf::Mouse::Right:
                        break;
                }
            }
        }

        HandleTimers(clock.restart().asMilliseconds());
        fpsClock.restart();
        window.clear(sf::Color(136, 247, 255));
        currLevel->DrawMap(window, gameState == STATE_MAIN_MENU);

        //! Temporarily commented out because it brings weird behavior to objects with Tile class
        //sf::Vector2f cameraPos = gameState == STATE_MAIN_MENU ? menuPlayer->GetPosition() : player->GetPosition();

        //if (cameraPos.x > window.getSize().x / 2.f)
        //    view.setCenter(cameraPos.x, view.getCenter().y);
        //else
        //    view.setCenter(window.getSize().x / 2.f, view.getCenter().y);

        //if (cameraPos.y > window.getSize().y / 2.f)
        //    view.setCenter(view.getCenter().x, cameraPos.y);
        //else
        //    view.setCenter(view.getCenter().x, window.getSize().y / 2.f);

        //window.setView(view);

        switch (gameState)
        {
            case STATE_MAIN_MENU:
            {
                menuPlayer->Update();
                menu->Update(window);

                if (menuPlayer->GetPositionX() > window.getSize().x / 2.f + 300.0f)
                    view.setCenter(menuPlayer->GetPositionX(), view.getCenter().y);
                else
                    view.setCenter(window.getSize().x / 2.f + 300.0f, view.getCenter().y);

                if (menuPlayer->GetPositionY() > window.getSize().y / 2.f)
                    view.setCenter(view.getCenter().x + 300.0f, menuPlayer->GetPositionY());
                else
                    view.setCenter(view.getCenter().x + 300.0f, window.getSize().y / 2.f);

                window.setView(view);
                break;
            }
            case STATE_PLAYING:
            {
                for (std::vector<Tile*>::iterator itr = allTiles.begin(); itr != allTiles.end(); ++itr)
                    (*itr)->Update();

                for (std::vector<Coin*>::iterator itr = allCoins.begin(); itr != allCoins.end(); ++itr)
                    if (!(*itr)->IsTaken())
                        (*itr)->Update();

                for (std::vector<Unit*>::iterator itr = allUnits.begin(); itr != allUnits.end(); ++itr)
                    if ((*itr)->GetTypeId() != TYPEID_MENU_PLAYER)
                        (*itr)->Update();

                for (std::vector<Bullet*>::iterator itr = allBullets.begin(); itr != allBullets.end(); ++itr)
                    if (!(*itr)->IsRemoved())
                        (*itr)->Update();

                //player->Update();
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
                //player->Update();
                player->DrawAccessoires(window, view);

                for (std::vector<Tile*>::iterator itr = allTiles.begin(); itr != allTiles.end(); ++itr)
                    (*itr)->Update();

                for (std::vector<Coin*>::iterator itr = allCoins.begin(); itr != allCoins.end(); ++itr)
                    if (!(*itr)->IsTaken())
                        (*itr)->Update();

                for (std::vector<Unit*>::iterator itr = allUnits.begin(); itr != allUnits.end(); ++itr)
                    if ((*itr)->GetTypeId() != TYPEID_MENU_PLAYER)
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

        if (showDebugInfo)
        {
            sf::Vector2f cameraPos = gameState == STATE_MAIN_MENU ? menuPlayer->GetPosition() : player->GetPosition();

            sf::Text text("Position X: " + std::to_string(static_cast<long long>(cameraPos.x)) + "\nPosition Y: " + std::to_string(static_cast<long long>(cameraPos.y)), font, 15);
            text.setColor(GAME_STATE_DRAW_GAME(gameState) || gameState == STATE_MAIN_MENU ? sf::Color::Black : sf::Color::White);
            text.setPosition(view.getCenter().x + 375.0f, view.getCenter().y - 300.0f);
            window.draw(text);

            float fps = 1 / fpsClock.getElapsedTime().asSeconds();
            sf::Text text2("FPS: " + std::to_string(static_cast<long long>(fps)), font, 15);
            text2.setColor(GAME_STATE_DRAW_GAME(gameState) || gameState == STATE_MAIN_MENU ? sf::Color::Black : sf::Color::White);
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

    for (std::vector<Unit*>::iterator itr = allUnits.begin(); itr != allUnits.end(); ++itr)
        if (!(*itr)->IsDead())
            (*itr)->HandleTimers(diff_time);

    for (std::vector<Tile*>::iterator itr = allTiles.begin(); itr != allTiles.end(); ++itr)
        if (!(*itr)->IsRemoved())
            (*itr)->HandleTimers(diff_time);
}

void Game::StartActualGame(sf::RenderWindow &window, std::string filename)
{
    gameState = STATE_PLAYING;
    currLevel->LoadMap("Levels/" + filename, window);
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
