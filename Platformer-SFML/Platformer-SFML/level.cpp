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
#include "enemy.h"
#include "coin.h"
#include "movingtile.h"
#include "bouncetile.h"
#include "bonustile.h"
#include "watertile.h"
#include "lavatile.h"
#include "quicksandtile.h"
#include "spiketile.h"
#include "Library\Dirent\include\dirent.h"

std::map<std::string /* filename */, sf::Texture> Level::Textures;

Level::Level()
{
    LoadAllImages();
    currLevel = 0;
}

Level::~Level()
{
    
}

void Level::LoadAllImages()
{
    DIR* dir;
    struct dirent *ent;
    std::stringstream ss;
    sf::Texture image;
    char const* dirSubName[4] = { "Graphics/Tiles", "Graphics/Character", "Graphics/Enemies", "Graphics/Other" };

    for (int i = 0; i < 4; ++i)
    {
        if ((dir = opendir(dirSubName[i])) != NULL)
        {
            while ((ent = readdir(dir)) != NULL)
            {
                if (ent->d_name[0] != '.') //! These seem to be the only hidden invisible files in there and the dirent library doesn't offer detection for it, so this will work. :)
                {
                    ss << dirSubName[i] << "/" << ent->d_name;
                    image.loadFromFile(ss.str().c_str());
                    Textures[ss.str().c_str()] = image;
                    ss.str(std::string());
                }
            }

            closedir(dir);
        }
    }
}

int Level::GetAmountOfTiles(std::string filename)
{
    std::ifstream openfile("Levels/level" + filename + ".txt");
    int amountOfTiles = 0;
    std::string line;

    while (std::getline(openfile, line))
        for (int i = 0; i < line.length(); i++)
            if (line[i] != ' ')
                amountOfTiles++;

    return amountOfTiles;
}

void Level::LoadMap(std::string filename, sf::RenderWindow &window, bool reload /* = false */)
{
    sf::Clock _clock;
    _clock.restart();
    std::vector<std::vector<std::string>> tilesInfoLayers;
    std::vector<std::string> tilesInfoBlocks;
    std::ifstream openfile("Levels/level" + filename + ".txt");
    std::stringstream lineStream;
    std::string line;
    currLevel = atoi(filename.c_str());
    bool foundPlayer = false;

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
    sGame.ClearGameObjects();
    sGame.ClearGameObjectCollidables();
    sGame.ClearCoins();
    sGame.ClearAllSpecialTiles();
    sGame.ClearAllEnemies();
    sGame.SetPlayer(NULL);

    for (int i = 0; i < tilesInfoLayers.size(); i++)
    {
        for (int j = 0; j < tilesInfoLayers[i].size(); j++)
        {
            sGame.LoadedAnotherTile();

            if (tilesInfoLayers[i][j] == "_")
                continue;

            sf::Vector2f mapPosition(j * 50.0f, i * 50.0f);

            if (tilesInfoLayers[i][j] == "A")
            {
                sGame.AddCoin(new Coin(&window, sf::Vector2f(j * 50.0f, i * 50.0f - 20.0f + float(urand(0, 9))), Textures["Graphics/Tiles/coin_gold.png"]));
                continue;
            }
            else if (tilesInfoLayers[i][j] == "B" || tilesInfoLayers[i][j] == "C")
            {
                sf::Vector2f startPos = mapPosition;
                sf::Vector2f destiPos = startPos;

                if (tilesInfoLayers[i][j] == "B")
                    destiPos.x += 200.0f;
                else
                {
                    startPos.y += 35.0f;
                    destiPos.y -= 200.0f;
                }

                sGame.AddSpecialTile(new MovingTile(&window, Textures["Graphics/Tiles/plank.png"], 3, startPos, destiPos, tilesInfoLayers[i][j] == "B"));
                continue;
            }
            else if (tilesInfoLayers[i][j] == "D")
            {
                sGame.AddSpecialTile(new BonusTile(&window, Textures["Graphics/Tiles/bonus.png"], Textures["Graphics/Tiles/bonus_used.png"], mapPosition));
                continue;
            }
            else if (tilesInfoLayers[i][j] == "Y")
            {
                sGame.AddSpecialTile(new QuickSandTile(&window, Textures["Graphics/Tiles/ground_sand.png"], mapPosition));
                continue;
            }
            else if (tilesInfoLayers[i][j] == "S")
            {
                mapPosition.y += 25.0f;
                sGame.AddSpecialTile(new WaterTile(&window, Textures["Graphics/Tiles/water.png"], mapPosition));
                continue;
            }
            else if (tilesInfoLayers[i][j] == "R")
            {
                mapPosition.y += 25.0f;
                sGame.AddSpecialTile(new LavaTile(&window, Textures["Graphics/Tiles/lava.png"], mapPosition));
                continue;
            }
            else if (tilesInfoLayers[i][j] == "E" || tilesInfoLayers[i][j] == "F")
            {
                std::string tileColor = tilesInfoLayers[i][j] == "E" ? "blue" : "red";
                sGame.AddSpecialTile(new BounceTile(&window, Textures["Graphics/Tiles/switch_" + tileColor + "_on.png"], Textures["Graphics/Tiles/switch_" + tileColor + "_off.png"], mapPosition, tileColor));
                continue;
            }
            else if (tilesInfoLayers[i][j] == "G")
            {
                mapPosition.y += 25.0f;
                sGame.AddSpecialTile(new SpikeTile(&window, Textures["Graphics/Tiles/spikes.png"], mapPosition));
                continue;
            }
            else if (tilesInfoLayers[i][j] == "!")
            {
                foundPlayer = true;

                if (!reload && sGame.GetPlayer())
                {
                    std::cout << "There is already a player defined in level " + filename + "." << std::endl;
                    continue;
                }

                sf::Texture imageCharacter;
                std::vector<std::pair<int, sf::Texture>> spriteCharactersLeft;
                std::vector<std::pair<int, sf::Texture>> spriteCharactersRight;

                for (int x = 0; x < 2; ++x)
                {
                    for (int z = 0; z < 10; ++z)
                    {
                        sf::Texture imageCharacter = Textures["Graphics/Character/walk_" + std::to_string(static_cast<long long>(z)) + "_" + (x ? "l" : "r") + ".png"];
                        x ? spriteCharactersRight.push_back(std::make_pair(z, imageCharacter)) : spriteCharactersLeft.push_back(std::make_pair(z, imageCharacter));
                    }
                }

                sGame.SetPlayer(new Player(&window, mapPosition, spriteCharactersLeft, spriteCharactersRight, Textures["Graphics/Other/heart_empty.png"], Textures["Graphics/Other/heart_full.png"], Textures["Graphics/Tiles/coin_gold_small.png"], Textures["Graphics/Character/jump_l.png"], Textures["Graphics/Character/jump_r.png"], Textures["Graphics/Other/bullet.png"]));
                continue;
            }
            else if (tilesInfoLayers[i][j] == "Z" || tilesInfoLayers[i][j] == "?")
            {
                bool slime = tilesInfoLayers[i][j] == "Z";
                std::vector<std::pair<int, sf::Texture>> spriteEnemiesLeft;
                std::vector<std::pair<int, sf::Texture>> spriteEnemiesRight;

                for (int x = 0; x < 2; ++x)
                {
                    for (int z = 0; z < 2; ++z)
                    {
                        if (slime)
                        {
                            sf::Texture imageEnemy = Textures["Graphics/Enemies/slime_" + std::string(z ? "walk_" : "normal_") + (x ? "l" : "r") + ".png"];
                            x ? spriteEnemiesRight.push_back(std::make_pair(z, imageEnemy)) : spriteEnemiesLeft.push_back(std::make_pair(z, imageEnemy));
                        }
                        else
                        {
                            sf::Texture imageEnemy = Textures["Graphics/Enemies/fly_" + std::string(z ? "fly_" : "normal_") + (x ? "l" : "r") + ".png"];
                            x ? spriteEnemiesRight.push_back(std::make_pair(z, imageEnemy)) : spriteEnemiesLeft.push_back(std::make_pair(z, imageEnemy));
                        }
                    }
                }

                sGame.AddEnemy(new Enemy(&window, mapPosition, spriteEnemiesLeft, spriteEnemiesRight, Textures["Graphics/Enemies/" + std::string(!slime ? "fly_dead" : "slime_dead") + ".png"], Textures["Graphics/Other/bullet.png"], 3, 1, 80, !slime));
                continue;
            }

            bool isCollidable = false;
            std::string fileName = "";

            if (tilesInfoLayers[i][j] == "I")
            {
                if (urand(0, 20) < 18)
                    continue;

                fileName = "Graphics/Tiles/cloud_" + std::to_string(static_cast<long long>(urand(0, 2))) + ".png";
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
            else if (tilesInfoLayers[i][j] == "T")
                fileName = "Graphics/Tiles/bush.png";
            else if (tilesInfoLayers[i][j] == "U")
                fileName = "Graphics/Tiles/grass.png";
            else if (tilesInfoLayers[i][j] == "V")
                fileName = "Graphics/Tiles/rock.png";
            else if (tilesInfoLayers[i][j] == "W")
                fileName = "Graphics/Tiles/shroom.png";
            else if (tilesInfoLayers[i][j] == "X")
            {
                isCollidable = true;
                fileName = "Graphics/Tiles/crate.png";
            }
            else
                std::cout << "Unkown type ID found in '" + fileName + "', letter '" + tilesInfoLayers[i][j] + "'." << std::endl;

            if (fileName == "")
                continue;

            if (!Textures.count(fileName))
            {
                std::cout << "Unloaded image ('" << fileName << "') found" << std::endl;
                continue;
            }

            SpriteInfo tileInfo;
            tileInfo.image = Textures[fileName];
            tileInfo.posX = mapPosition.x;
            tileInfo.posY = mapPosition.y;
            sprites.push_back(tileInfo);

            sf::Sprite tmpSprite;
            tmpSprite.setTexture(Textures[fileName]);
            tmpSprite.setPosition(mapPosition);

            if (isCollidable)
                sGame.AddGameObjectCollidable(tmpSprite);

            sGame.AddGameObject(tmpSprite);
        }
    }

    std::cout << "Time in milliseconds taken to load level '" + filename + "': " << _clock.restart().asMilliseconds() << std::endl;

    if (!foundPlayer)
        std::cout << "No player found in level " + filename + "." << std::endl;

    sGame.SetLoadedTiles(0);
}

void Level::DrawMap(sf::RenderWindow &window, bool menuLevel /* = false */)
{
    sf::Vector2f cameraPos = menuLevel ? sGame.GetMenuPlayer()->GetPosition() : sGame.GetPlayer()->GetPosition();
    float distToCamera = menuLevel ? 1600.0f : 1000.0f;

    for (std::vector<SpriteInfo>::iterator itr = sprites.begin(); itr != sprites.end(); ++itr)
    {
        //! ONLY draw the images if the player is within visibility distance, else there's no point in wasting performance.
        if (IsInRange(cameraPos.x, (*itr).posX, cameraPos.y, (*itr).posY, distToCamera))
        {
            sf::Sprite sprite((*itr).image);
            sprite.setPosition((*itr).posX, (*itr).posY);

            if (GAME_STATE_PAUSED(sGame.GetGameState()))
                sprite.setColor(sf::Color(255, 255, 255, 128));

            window.draw(sprite);
        }
    }
}
