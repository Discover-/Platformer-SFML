#include <fstream>
#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "level.h"
#include "shareddefines.h"
#include "player.h"
#include "game.h"
#include "coin.h"
#include "movingtile.h"
#include "bouncetile.h"

Level::Level(Game* _game)
{
    game = _game;
}

Level::~Level()
{
    
}

void Level::LoadMap(char const* filename, sf::RenderWindow &window)
{
    std::vector<std::vector<std::string>> tilesInfoLayers;
    std::vector<std::string> tilesInfoBlocks;
    std::ifstream openfile(filename);
    std::stringstream lineStream;
    std::string line;

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

    game->ClearGameObjects();
    game->ClearGameObjectCollidables();
    sprites.clear();
    game->ClearCoins();
    game->ClearAllTiles();

    for (int i = 0; i < tilesInfoLayers.size(); i++)
    {
        for (int j = 0; j < tilesInfoLayers[i].size(); j++)
        {
            if (tilesInfoLayers[i][j] == "_")
                continue;

            if (tilesInfoLayers[i][j] == "P")
            {
                game->AddCoin(new Coin(game, &window, sf::Vector2f(j * 50.0f, i * 50.0f - 20.0f)));
                continue;
            }
            else if (tilesInfoLayers[i][j] == "Y" || tilesInfoLayers[i][j] == "R")
            {
                sf::Vector2f startPos(j * 50.0f, i * 50.0f);
                sf::Vector2f destiPos = startPos;

                if (tilesInfoLayers[i][j] == "Y")
                    destiPos.x += 200.0f;
                else
                {
                    startPos.y += 35.0f;
                    destiPos.y -= 200.0f;
                }

                sf::Texture image;
                image.loadFromFile("Graphics/Tiles/plank.png");
                game->AddTile(new MovingTile(game, &window, image, 3, startPos, destiPos, tilesInfoLayers[i][j] == "Y"));
                continue;
            }
            else if (tilesInfoLayers[i][j] == "Q" || tilesInfoLayers[i][j] == "H" || tilesInfoLayers[i][j] == "U" || tilesInfoLayers[i][j] == "R")
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

            if (tilesInfoLayers[i][j] == "0")
            {
                if (urand(0, 20) > 18)
                    fileName = "Graphics/Tiles/cloud_" + std::to_string(static_cast<long long>(urand(0, 2))) + ".png";
                else
                    continue;
            }
            else if (tilesInfoLayers[i][j] == "1")
            {
                isCollidable = true;
                fileName = "Graphics/Tiles/block.png";
            }
            else if (tilesInfoLayers[i][j] == "2")
            {
                isCollidable = true;
                fileName = "Graphics/Tiles/ground.png";
            }
            else if (tilesInfoLayers[i][j] == "3")
            {
                isCollidable = true;
                fileName = "Graphics/Tiles/ground_dirt.png";
            }
            else if (tilesInfoLayers[i][j] == "4")
            {
                isCollidable = true;
                fileName = "Graphics/Tiles/bridge.png";
            }
            //else if (tilesInfoLayers[i][j] == "5")
            //    fileName = "Graphics/Tiles/.png";
            else if (tilesInfoLayers[i][j] == "6")
                fileName = "Graphics/Tiles/hill_short.png";
            else if (tilesInfoLayers[i][j] == "7")
                fileName = "Graphics/Tiles/hill_long.png";
            else if (tilesInfoLayers[i][j] == "8")
                fileName = "Graphics/Tiles/fence_normal.png";
            else if (tilesInfoLayers[i][j] == "9")
                fileName = "Graphics/Tiles/fence_broken.png";
            else if (tilesInfoLayers[i][j] == "L")
                fileName = "Graphics/Tiles/lava.png";
            else if (tilesInfoLayers[i][j] == "W")
                fileName = "Graphics/Tiles/water.png";
            else if (tilesInfoLayers[i][j] == "B")
                fileName = "Graphics/Tiles/bush.png";
            else if (tilesInfoLayers[i][j] == "G")
                fileName = "Graphics/Tiles/grass.png";
            else if (tilesInfoLayers[i][j] == "R")
                fileName = "Graphics/Tiles/rock.png";
            else if (tilesInfoLayers[i][j] == "S")
                fileName = "Graphics/Tiles/shroom.png";
            else if (tilesInfoLayers[i][j] == "C")
                fileName = "Graphics/Tiles/crate.png";
            else if (tilesInfoLayers[i][j] == "Z")
            {
                isCollidable = true;
                fileName = "Graphics/Tiles/bonus.png";
            }
            else if (tilesInfoLayers[i][j] == "D")
            {
                isCollidable = true;
                fileName = "Graphics/Tiles/ground_sand.png";
            }

            if (fileName == "")
                continue;

            sf::Texture image;
            image.loadFromFile(fileName);
            SpriteInfo tileInfo;
            tileInfo.image = image;
            tileInfo.posX = j * 50.0f;
            tileInfo.posY = i * 50.0f;
            sprites.push_back(tileInfo);

            sf::Sprite tmpSprite(image);
            tmpSprite.setPosition(j * 50.0f, i * 50.0f);

            if (isCollidable)
                game->AddGameObjectCollidable(tmpSprite);

            if (tilesInfoLayers[i][j] == "D")
                game->AddQuickSandObject(tmpSprite);
            else if (tilesInfoLayers[i][j] == "W")
                game->AddWaterObject(tmpSprite);
            else if (tilesInfoLayers[i][j] == "L")
                game->AddLavaObject(tmpSprite);

            game->AddGameObject(tmpSprite);
        }
    }
}

void Level::DrawMap(sf::RenderWindow &window)
{
    Player* player = game->GetPlayer();

    for (std::vector<SpriteInfo>::iterator itr = sprites.begin(); itr != sprites.end(); ++itr)
    {
        //! ONLY draw the images if the player is within visibility distance, else there's no point in wasting performance.
        if (IsInRange(player->GetPositionX(), (*itr).posX, player->GetPositionY(), (*itr).posY, 950.0f))
        {
            sf::Sprite sprite((*itr).image);
            sprite.setPosition((*itr).posX, (*itr).posY);

            if (GAME_STATE_PAUSED_DRAWING(game->GetGameState()))
                sprite.setColor(sf::Color(255, 255, 255, 128));

            window.draw(sprite);
        }
    }
}
