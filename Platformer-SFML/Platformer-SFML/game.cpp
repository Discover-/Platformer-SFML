#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <time.h>
#include <unordered_map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
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
#include <dirent.h>
#include "sound.h"

std::map<std::string /* filename */, Sound*> Game::Sounds;

Game::Game()
{
    isRunning = true;
    showDebugInfo = true;
    gameState = STATE_MAIN_MENU;
    player = NULL;
    menuPlayer = NULL;
}

Game::~Game()
{
    DeleteContentMemory();
    delete currLevel;
}

void Game::DeleteContentMemory()
{
    for (std::vector<Unit*>::iterator itr = allUnits.begin(); itr != allUnits.end(); ++itr)
        delete *itr;

    for (std::vector<Bullet*>::iterator itr = allBullets.begin(); itr != allBullets.end(); ++itr)
        delete *itr;

    for (std::vector<SpecialTile*>::iterator itr = allSpecialTiles.begin(); itr != allSpecialTiles.end(); ++itr)
        delete *itr;

    for (std::vector<Coin*>::iterator itr = allCoins.begin(); itr != allCoins.end(); ++itr)
        delete *itr;

    //for (std::map<std::string /* filename */, Sound*>::iterator itr = Sounds.begin(); itr != Sounds.end(); ++itr)
    //    delete (*itr).second;
}

void Game::LoadAllSounds()
{
    DIR* dir;
    struct dirent* ent;
    std::stringstream ss;

    if ((dir = opendir("Sounds")) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] != '.') //! These seem to be the only hidden invisible files in there and the dirent library doesn't offer detection for it, so this will work. :)
            {
                Sound* sound = new Sound();
                ss << "Sounds/" << ent->d_name;

                if (sound->Load(ss.str().c_str()))
                    Sounds[ss.str()] = sound;

                ss.str(std::string());
            }
        }

        closedir(dir);
    }
}

int Game::Update()
{
    sf::Clock clockStart;
    clockStart.restart();

    sf::RenderWindow window(sf::VideoMode(1000, 600), "Platformer C++ SFML", sf::Style::Close);
    window.setFramerateLimit(30);

    sf::Font font;
    font.loadFromFile("Fonts/Market_Deco.ttf");

    sf::View view(window.getDefaultView());
    sf::Clock clock, fpsClock;

    LoadAllSounds();

    //! Level::Level calls Level::LoadMap which then initializes Game::Player so we can access the Player's class in order to get its sprites!
    currLevel = new Level(this, window);

    menuPlayer = new MenuPlayer(this, &window, sf::Vector2f(165.0f, 285.0f), player->GetSpritesLeft(), sf::Texture());
    allUnits.push_back(menuPlayer);

    Menu* menu = new Menu(this);
    menu->Load();

    std::cout << "Time in milliseconds taken to load everything before entering while-loop: " << clockStart.restart().asMilliseconds() << std::endl;

    float playedTime = 0.0f;
    bool justGainedFocus = false;

    while (window.isOpen())
    {
        sf::Event _event;

        bool shiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

        while (window.pollEvent(_event))
        {
            switch (_event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonReleased:
                    if (_event.mouseButton.button == sf::Mouse::Left && shiftPressed)
                    {
                        std::cout << "Mouse X: " << sf::Mouse::getPosition(window).x << std::endl;
                        std::cout << "Mouse Y: " << sf::Mouse::getPosition(window).y << std::endl;
                    }
                    break;
                case sf::Event::LostFocus:
                    if (gameState == STATE_PLAYING)
                        gameState = STATE_PAUSED_FOCUS;
                    break;
                case sf::Event::GainedFocus:
                    justGainedFocus = true;

                    if (gameState == STATE_PAUSED_FOCUS)
                        gameState = STATE_PLAYING;
                    break;
                case sf::Event::KeyReleased:
                {
                    switch (_event.key.code)
                    {
                        //! Reload map
                        case sf::Keyboard::F1:
                        {
                            sf::Clock _clock; _clock.restart();
                            std::stringstream ss; ss << currLevel->GetCurrentLevel();
                            currLevel->LoadMap(ss.str(), window, true);
                            break;
                        }
                        //! Back to menu
                        case sf::Keyboard::F2:
                        {
                            menu->SetCurrentMenu(MENU_MAIN);
                            gameState = STATE_MAIN_MENU;
                            currLevel->LoadMap("menu", window);
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
                                gameState = gameState == STATE_PLAYING ? STATE_PAUSED : STATE_PLAYING;
                            else
                                window.close();
                            break;
                        //! Move menu option selection up
                        case sf::Keyboard::Up:
                            if (gameState == STATE_MAIN_MENU)
                                menu->UpdateSelection(true);
                            break;
                        //! Move menu option selection down
                        case sf::Keyboard::Down:
                            if (gameState == STATE_MAIN_MENU)
                                menu->UpdateSelection(false);
                            break;
                        //! Move menu option selection to left
                        case sf::Keyboard::Left:
                            if (gameState == STATE_MAIN_MENU && menu->GetCurrentMenu() == MENU_LEVELS)
                                menu->UpdateSelection(true);
                            break;
                        //! Move menu option selection to right
                        case sf::Keyboard::Right:
                            if (gameState == STATE_MAIN_MENU && menu->GetCurrentMenu() == MENU_LEVELS)
                                menu->UpdateSelection(false);
                            break;
                        //! Select menu option
                        case sf::Keyboard::Return:
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
                    break;
                }
                case sf::Event::MouseWheelMoved:
                    if (gameState == STATE_MAIN_MENU)
                        menu->UpdateSelection(_event.mouseWheel.delta > 0);
                    break;
                case sf::Event::MouseButtonPressed:
                {
                    if (justGainedFocus)
                    {
                        justGainedFocus = false;
                        break;
                    }

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
                    break;
                }
            }
        }

        float timer = clock.restart().asSeconds();
        HandleTimers(sf::Int32(timer * 1000));
        fpsClock.restart();
        window.clear(sf::Color(136, 247, 255));
        currLevel->DrawMap(window, gameState == STATE_MAIN_MENU);

        //! Temporarily commented out because it brings weird behavior to objects with SpecialTile class
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

                if (menuPlayer->GetPositionX() > window.getSize().x / 2.f + 400.0f)
                    view.setCenter(menuPlayer->GetPositionX(), view.getCenter().y);
                else
                    view.setCenter(window.getSize().x / 2.f + 400.0f, view.getCenter().y);

                if (menuPlayer->GetPositionY() > window.getSize().y / 2.f)
                    view.setCenter(view.getCenter().x + 400.0f, menuPlayer->GetPositionY());
                else
                    view.setCenter(view.getCenter().x + 400.0f, window.getSize().y / 2.f);

                window.setView(view);
                break;
            }
            case STATE_PLAYING:
            {
                for (std::vector<SpecialTile*>::iterator itr = allSpecialTiles.begin(); itr != allSpecialTiles.end(); ++itr)
                    (*itr)->Update();

                for (std::vector<Coin*>::iterator itr = allCoins.begin(); itr != allCoins.end(); ++itr)
                    if (!(*itr)->IsTaken())
                        (*itr)->Update();

                for (std::vector<Unit*>::iterator itr = allUnits.begin(); itr != allUnits.end(); ++itr)
                    if ((*itr)->GetTypeId() != TYPEID_MENU_PLAYER)
                        (*itr)->Update();

                if (!allBullets.empty())
                    for (std::vector<Bullet*>::iterator itr = allBullets.begin(); itr != allBullets.end(); ++itr)
                        if (!(*itr)->IsRemoved())
                            (*itr)->Update();

                //player->Update();
                player->DrawAccessoires(window, view);

                float cameraX = 0.0f, cameraY = 0.0f;

                if (player->GetPositionX() > window.getSize().x / 2.f || player->GetPositionX() < window.getSize().x / 2.f)
                    cameraX = player->GetPositionX();
                else
                    cameraX = window.getSize().x / 2.f;

                if (player->GetPositionY() > window.getSize().y / 2.f || player->GetPositionY() < window.getSize().y / 2.f)
                    cameraY = player->GetPositionY();
                else
                    cameraY = window.getSize().y / 2.f;

                view.setCenter(cameraX, cameraY);

                window.setView(view);
                break;
            }
            case STATE_PAUSED:
            case STATE_PAUSED_FOCUS:
            {
                //player->Update();
                player->DrawAccessoires(window, view);

                for (std::vector<SpecialTile*>::iterator itr = allSpecialTiles.begin(); itr != allSpecialTiles.end(); ++itr)
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

            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            sf::Text text3("Mouse X: " + std::to_string(static_cast<long long>(mousePos.x)) + "\nMouse Y: " + std::to_string(static_cast<long long>(mousePos.y)), font, 15);
            text3.setColor(GAME_STATE_DRAW_GAME(gameState) || gameState == STATE_MAIN_MENU ? sf::Color::Black : sf::Color::White);
            text3.setPosition(view.getCenter().x + 375.0f, view.getCenter().y - 265.0f);
            window.draw(text3);

            float fps = 1 / fpsClock.getElapsedTime().asSeconds();
            sf::Text text2("FPS: " + std::to_string(static_cast<long long>(fps)), font, 15);
            text2.setColor(GAME_STATE_DRAW_GAME(gameState) || gameState == STATE_MAIN_MENU ? sf::Color::Black : sf::Color::White);
            text2.setPosition(view.getCenter().x + 375.0f, view.getCenter().y - 230.0f);
            window.draw(text2);
        }

        if (gameState != STATE_MAIN_MENU)
        {
            sf::Text text("Level " + std::to_string(static_cast<long long>(currLevel->GetCurrentLevel())) + " / 7", font, 30);
            text.setColor(GAME_STATE_DRAW_GAME(gameState) || gameState == STATE_MAIN_MENU ? sf::Color::Black : sf::Color::White);
            text.setPosition(view.getCenter().x + 330.0f, view.getCenter().y + 255.0f);
            window.draw(text);

            sf::Text text2("Timer " + std::to_string(static_cast<long long>(GAME_STATE_PAUSED(gameState) ? playedTime : playedTime += timer)), font, 30);
            text2.setColor(GAME_STATE_DRAW_GAME(gameState) || gameState == STATE_MAIN_MENU ? sf::Color::Black : sf::Color::White);
            text2.setPosition(view.getCenter().x + 330.0f, view.getCenter().y + 225.0f);
            window.draw(text2);
        }

        for (std::map<std::string /* filename */, Sound*>::iterator itr = Sounds.begin(); itr != Sounds.end(); ++itr)
            (*itr).second->Update();

        window.display();
    }

    delete menu;
    return 0;
}

void Game::HandleTimers(sf::Int32 diff_time)
{
    //if (gameState != STATE_PLAYING)
    //    return;

    for (std::vector<Unit*>::iterator itr = allUnits.begin(); itr != allUnits.end(); ++itr)
        if (!(*itr)->IsDead())
            (*itr)->HandleTimers(diff_time);

    for (std::vector<SpecialTile*>::iterator itr = allSpecialTiles.begin(); itr != allSpecialTiles.end(); ++itr)
        if (!(*itr)->IsRemoved())
            (*itr)->HandleTimers(diff_time);
}

void Game::StartActualGame(sf::RenderWindow &window, std::string filename)
{
    gameState = STATE_PLAYING;
    currLevel->LoadMap(filename, window);
}

void Game::RemoveUnitWithTypeId(UnitTypeId typeId)
{
    for (std::vector<Unit*>::iterator itr = allUnits.begin(); itr != allUnits.end();)
    {
        if ((*itr)->GetTypeId() == typeId)
        {
            allUnits.erase(itr);
            itr = allUnits.begin();
        }
        else
            ++itr;
    }
}

void Game::RemoveBullet(Bullet* bullet)
{
    //if (!bullet || allBullets.empty())
    //    return;

    //for (std::vector<Bullet*>::iterator itr = allBullets.begin(); itr != allBullets.end();)
    //{
    //    if ((*itr) == bullet)
    //    {
    //        allBullets.erase(itr);
    //        itr = allBullets.begin();
    //    }
    //    else
    //        ++itr;
    //}

    //delete bullet;
}
