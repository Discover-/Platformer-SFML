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
#include "Library\Dirent\include\dirent.h"
#include "audio.h"
#include <thread>

Game::Game()
{
    isRunning = true;
    showDebugInfo = true;
    gameState = STATE_LOADING_LEVEL;
    upcomingGameState = STATE_NONE;
    player = NULL;
    menuPlayer = NULL;
    mutedMusic = false;
    loadedTiles = 0;
    currentlyLoadingLvl = "";
}

Game::~Game()
{
    DeleteContentMemory();
    delete audio;
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
}

void Game::LoadAllAudio()
{
    DIR* dir;
    struct dirent* ent;
    std::stringstream ss;
    audio = new Audio(this);

    if ((dir = opendir("Audio")) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] != '.') //! These seem to be the only hidden invisible files in there and the dirent library doesn't offer detection for it, so this will work. :)
            {
                ss << "Audio/" << ent->d_name;
                audio->Load(ss.str().c_str());
                ss.str(std::string());
            }
        }

        closedir(dir);
    }
}

class LevelMapLoader
{
    public:
        explicit LevelMapLoader(Game* _game, Level* _currLevel, std::string _filename, sf::RenderWindow* _window) : game(_game), currLevel(_currLevel), filename(_filename), window(_window) { }

        void operator()()
        {
            window->setView(window->getDefaultView());

            //! Level::LoadMap initializes Game::player so we can access the Player's class in order to get its sprites! (player->GetSpritesLeft())
            currLevel->LoadMap(filename, *window);
            MenuPlayer* menuPlayer = new MenuPlayer(game, window, sf::Vector2f(165.0f, 285.0f), game->GetPlayer()->GetSpritesLeft(), sf::Texture());
            game->SetMenuPlayer(menuPlayer);
            game->AddUnit(menuPlayer);
            game->SetUpcomingGameState(filename == "menu" ? STATE_MAIN_MENU : STATE_PLAYING);
            game->SetCurrentlyLoadingLvl("");
        }
        
    private:
        Game* game;
        Level* currLevel;
        std::string filename;
        sf::RenderWindow* window;
};

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

    LoadAllAudio();

    currLevel = new Level(this);

    currentlyLoadingLvl = "menu";
    std::thread threadLevelMapLoader(LevelMapLoader(this, currLevel, "menu", &window));
    threadLevelMapLoader.detach();

    Menu* menu = new Menu(this);
    menu->Load();

    std::cout << "Time in milliseconds taken to load everything before entering while-loop: " << clockStart.restart().asMilliseconds() << std::endl;

    float playedTime = 0.0f;
    bool justGainedFocus = false;

    while (window.isOpen())
    {
        window.clear(sf::Color(136, 247, 255));

        if (upcomingGameState != STATE_NONE && upcomingGameState != gameState)
        {
            gameState = upcomingGameState;
            upcomingGameState = STATE_NONE;
        }

        if (gameState == STATE_LOADING_LEVEL)
        {
            int amountOfTiles = currLevel->GetAmountOfTiles(currentlyLoadingLvl);
            float pct = floor(float(float(loadedTiles) / amountOfTiles) * 100);

            sf::RectangleShape loadingBarBackground;
            loadingBarBackground.setSize(sf::Vector2f(800.0f, 50.0f));
            loadingBarBackground.setFillColor(sf::Color(136, 247, 255));
            loadingBarBackground.setPosition(100.0f, 250.0f);
            loadingBarBackground.setOutlineColor(sf::Color::Black);
            loadingBarBackground.setOutlineThickness(2.0f);
            window.draw(loadingBarBackground);

            sf::RectangleShape loadingBarProgress;
            loadingBarProgress.setSize(sf::Vector2f(800.0f * (pct / 100.0f), 50.0f));
            loadingBarProgress.setFillColor(sf::Color::Green);
            loadingBarProgress.setPosition(100.0f, 250.0f);
            window.draw(loadingBarProgress);

            std::stringstream ssPct;
            ssPct << "Percentage: " << pct;
            sf::Text textPercentage(ssPct.str(), font, 25);
            textPercentage.setColor(sf::Color::White);
            textPercentage.setPosition(500.0f - (textPercentage.getLocalBounds().width / 2.0f), 270.0f - (textPercentage.getLocalBounds().height / 2.0f));
            window.draw(textPercentage);

            std::stringstream ssTiles;
            ssTiles << loadedTiles << " / " << amountOfTiles;
            sf::Text textTiles(ssTiles.str(), font, 15);
            textTiles.setColor(sf::Color::White);
            textTiles.setPosition(500.0f - (textTiles.getLocalBounds().width / 2.0f), 315.0f - (textTiles.getLocalBounds().height / 2.0f));
            window.draw(textTiles);

            window.display();
            continue;
        }

        sf::Event _event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        float timer = clock.restart().asSeconds();
        HandleTimers(sf::Int32(timer * 1000));
        fpsClock.restart();

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
                            std::stringstream ss;
                            ss << currLevel->GetCurrentLevel();
                            currentlyLoadingLvl = ss.str();
                            std::thread threadLevelMapLoader2(LevelMapLoader(this, currLevel, ss.str(), &window));
                            threadLevelMapLoader2.detach();
                            gameState = STATE_LOADING_LEVEL;
                            break;
                        }
                        //! Back to menu
                        case sf::Keyboard::F2:
                        {
                            currentlyLoadingLvl = "menu";
                            menu->SetCurrentMenu(MENU_MAIN);
                            std::thread threadLevelMapLoader2(LevelMapLoader(this, currLevel, "menu", &window));
                            threadLevelMapLoader2.detach();
                            gameState = STATE_LOADING_LEVEL;
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
                            if (GAME_STATE_MENU(gameState))
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
                            if (mousePos.x < 60 && mousePos.y > 545)
                            {
                                audio->SetVolume("all", mutedMusic ? 100.0f : 0.0f);
                                mutedMusic = !mutedMusic;
                            }
                            else if (gameState == STATE_MAIN_MENU)
                                menu->PressedEnterOrMouse(window);
                            break;
                        case sf::Mouse::Right:
                            break;
                    }
                    break;
                }
            }
        }

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

        sf::Sprite volume(Level::Textures["Graphics/Other/volume" + std::string(mutedMusic ? "_mute" : "") + ".png"]);
        volume.setPosition(view.getCenter().x - 490.0f, view.getCenter().y + 245.0f);

        if (!(mousePos.x < 60 && mousePos.y > 545))
            volume.setColor(sf::Color(255, 255, 255, 128));

        window.draw(volume);

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
    gameState = STATE_LOADING_LEVEL;
    currentlyLoadingLvl = filename;
    std::thread threadLevelMapLoader(LevelMapLoader(this, currLevel, filename, &window));
    threadLevelMapLoader.detach();
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
