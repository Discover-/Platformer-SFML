#include <fstream>
#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "level.h"
#include "shareddefines.h"
#include "player.h"
#include "menuplayer.h"
#include "game.h"
#include "coin.h"
#include "movingtile.h"
#include "bouncetile.h"
#include "bonustile.h"

Level::Level(Game* _game, sf::RenderWindow &window)
{
    game = _game;
    LoadMap("menu", window);
    currLevel = 0;
}

Level::~Level()
{
    
}

void Level::LoadMap(std::string filename, sf::RenderWindow &window)
{
    sf::Clock _clock;
    _clock.restart();
    std::vector<std::vector<std::string>> tilesInfoLayers;
    std::vector<std::string> tilesInfoBlocks;
    std::ifstream openfile("Levels/level" + filename + ".txt");
    std::stringstream lineStream;
    std::string line;
    currLevel = atoi(filename.c_str());

    while (std::getline(openfile, line))
    {
        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] != ' ')
            {
                lineStream << line[i];
                tilesInfoBlocks.push_back(lineStream.str());
                lineStream.str(std::string());
            }
        }

        tilesInfoLayers.push_back(tilesInfoBlocks);
        tilesInfoBlocks.clear();
    }

    sprites.clear();
    game->ClearGameObjects();
    game->ClearGameObjectCollidables();
    game->ClearCoins();
    game->ClearAllTiles();

    //! Last used letter: 
    for (int i = 0; i < tilesInfoLayers.size(); i++)
    {
        for (int j = 0; j < tilesInfoLayers[i].size(); j++)
        {
            if (tilesInfoLayers[i][j] == "_")
                continue;

            sf::Sprite tmpSprite;
            tmpSprite.setPosition(j * 50.0f, i * 50.0f);

            if (tilesInfoLayers[i][j] == "A")
            {
                game->AddCoin(new Coin(game, &window, sf::Vector2f(j * 50.0f, i * 50.0f - 20.0f + float(urand(0, 9)))));
                continue;
            }
            else if (tilesInfoLayers[i][j] == "B" || tilesInfoLayers[i][j] == "C")
            {
                sf::Vector2f startPos(j * 50.0f, i * 50.0f);
                sf::Vector2f destiPos = startPos;

                if (tilesInfoLayers[i][j] == "B")
                    destiPos.x += 200.0f;
                else
                {
                    startPos.y += 35.0f;
                    destiPos.y -= 200.0f;
                }

                sf::Texture image;
                image.loadFromFile("Graphics/Tiles/plank.png");
                game->AddTile(new MovingTile(game, &window, image, 3, startPos, destiPos, tilesInfoLayers[i][j] == "B"));
                //tmpSprite.setTexture(image);
                //game->AddGameObjectCollidable(tmpSprite);
                continue;
            }
            else if (tilesInfoLayers[i][j] == "D")
            {
                sf::Vector2f startPos(j * 50.0f, i * 50.0f);
                sf::Vector2f destiPos = startPos;
                sf::Texture image;
                image.loadFromFile("Graphics/Tiles/bonus.png");
                game->AddTile(new BonusTile(game, &window, image, startPos));
                continue;
            }
            else if (tilesInfoLayers[i][j] == "E" || tilesInfoLayers[i][j] == "F" || tilesInfoLayers[i][j] == "G" || tilesInfoLayers[i][j] == "H")
            {
                sf::Vector2f startPos(j * 50.0f, i * 50.0f);
                sf::Vector2f destiPos = startPos;
                std::string tileColor = GetBounceTileColor(tilesInfoLayers[i][j]);
                sf::Texture image;
                image.loadFromFile("Graphics/Tiles/switch_" + tileColor + "_off.png");
                game->AddTile(new BounceTile(game, &window, image, 3, startPos, destiPos, tileColor));
                continue;
            }

            bool isCollidable = false;
            std::string fileName = "";

            if (tilesInfoLayers[i][j] == "I")
            {
                if (urand(0, 20) > 18)
                    fileName = "Graphics/Tiles/cloud_" + std::to_string(static_cast<long long>(urand(0, 2))) + ".png";
                else
                    continue;
            }
            else if (tilesInfoLayers[i][j] == "J")
            {
                isCollidable = true;
                fileName = "Graphics/Tiles/block.png";
            }
            else if (tilesInfoLayers[i][j] == "K")
            {
                isCollidable = true;
                fileName = "Graphics/Tiles/ground.png";
            }
            else if (tilesInfoLayers[i][j] == "L")
            {
                isCollidable = true;
                fileName = "Graphics/Tiles/ground_dirt.png";
            }
            else if (tilesInfoLayers[i][j] == "M")
            {
                isCollidable = true;
                fileName = "Graphics/Tiles/bridge.png";
            }
            else if (tilesInfoLayers[i][j] == "N")
                fileName = "Graphics/Tiles/hill_short.png";
            else if (tilesInfoLayers[i][j] == "O")
                fileName = "Graphics/Tiles/hill_long.png";
            else if (tilesInfoLayers[i][j] == "P")
                fileName = "Graphics/Tiles/fence_normal.png";
            else if (tilesInfoLayers[i][j] == "Q")
                fileName = "Graphics/Tiles/fence_broken.png";
            else if (tilesInfoLayers[i][j] == "R")
                fileName = "Graphics/Tiles/lava.png";
            else if (tilesInfoLayers[i][j] == "S")
                fileName = "Graphics/Tiles/water.png";
            else if (tilesInfoLayers[i][j] == "T")
                fileName = "Graphics/Tiles/bush.png";
            else if (tilesInfoLayers[i][j] == "U")
                fileName = "Graphics/Tiles/grass.png";
            else if (tilesInfoLayers[i][j] == "V")
                fileName = "Graphics/Tiles/rock.png";
            else if (tilesInfoLayers[i][j] == "W")
                fileName = "Graphics/Tiles/shroom.png";
            else if (tilesInfoLayers[i][j] == "X")
                fileName = "Graphics/Tiles/crate.png";
            else if (tilesInfoLayers[i][j] == "Y")
            {
                isCollidable = true;
                fileName = "Graphics/Tiles/ground_sand.png";
            }
            else
                std::cout << "Unkown type ID found in '" + fileName + "', letter '" + tilesInfoLayers[i][j] + "'." << std::endl;

            if (fileName == "")
                continue;

            sf::Texture image;
            image.loadFromFile(fileName);
            SpriteInfo tileInfo;
            tileInfo.image = image;
            tileInfo.posX = j * 50.0f;
            tileInfo.posY = i * 50.0f;
            sprites.push_back(tileInfo);

            tmpSprite.setTexture(image);
            tmpSprite.setPosition(j * 50.0f, i * 50.0f);

            if (isCollidable)
                game->AddGameObjectCollidable(tmpSprite);

            if (tilesInfoLayers[i][j] == "Y")
                game->AddQuickSandObject(tmpSprite);
            else if (tilesInfoLayers[i][j] == "S")
                game->AddWaterObject(tmpSprite);
            else if (tilesInfoLayers[i][j] == "R")
                game->AddLavaObject(tmpSprite);

            game->AddGameObject(tmpSprite);
        }
    }

    std::cout << "Time in milliseconds taken to load level: " << _clock.restart().asMilliseconds() << std::endl;
}

void Level::DrawMap(sf::RenderWindow &window, bool menuLevel /* = false */)
{
    sf::Vector2f cameraPos = menuLevel ? game->GetMenuPlayer()->GetPosition() : game->GetPlayer()->GetPosition();
    float distToCamera = menuLevel ? 1600.0f : 950.0f;

    for (std::vector<SpriteInfo>::iterator itr = sprites.begin(); itr != sprites.end(); ++itr)
    {
        //! ONLY draw the images if the player is within visibility distance, else there's no point in wasting performance.
        if (IsInRange(cameraPos.x, (*itr).posX, cameraPos.y, (*itr).posY, distToCamera))
        {
            sf::Sprite sprite((*itr).image);
            sprite.setPosition((*itr).posX, (*itr).posY);

            if (GAME_STATE_PAUSED(game->GetGameState()))
                sprite.setColor(sf::Color(255, 255, 255, 128));

            window.draw(sprite);
        }
    }
}
