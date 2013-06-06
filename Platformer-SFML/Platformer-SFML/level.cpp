#include <fstream>
#include <string>
#include "level.h"
#include "shareddefines.h"
#include "player.h"
#include "game.h"

Level::Level(Game* _game)
{
    game = _game;
}

Level::~Level()
{
    
}

void Level::LoadMap(char const* filename)
{
    std::vector<std::vector<int>> tilesInfoLayers;
    std::vector<int> tilesInfoBlocks;
    std::ifstream openfile(filename);
    std::string line;

    while (std::getline(openfile, line))
    {
        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] != ' ')
            {
                char str = line[i];
                tilesInfoBlocks.push_back(atoi(&str));
            }
        }

        tilesInfoLayers.push_back(tilesInfoBlocks);
        tilesInfoBlocks.clear();
    }

    for (int i = 0; i < tilesInfoLayers.size(); i++)
    {
        for (int j = 0; j < tilesInfoLayers[i].size(); j++)
        {
            std::string fileName = GetTileFilename(tilesInfoLayers[i][j]);
            sf::Texture image;
            image.loadFromFile(fileName);
            SpriteInfo tileInfo;
            tileInfo.image = image;
            tileInfo.posX = j * 50.0f;
            tileInfo.posY = i * 50.0f;
            sprites.push_back(tileInfo);

            sf::Sprite tmpSprite(image);
            tmpSprite.setPosition(j * 50.0f, i * 50.0f);

            if (tilesInfoLayers[i][j] != 9 && tilesInfoLayers[i][j] != 0)
                game->AddGameObjectCollidable(tmpSprite);

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
        if (IsInRange(player->GetPositionX(), (*itr).posX, player->GetPositionY(), (*itr).posY, 1000.0f))
        {
            sf::Sprite sprite((*itr).image);
            sprite.setPosition((*itr).posX, (*itr).posY);

            if (GAME_STATE_PAUSED_DRAWING(game->GetGameState()))
                sprite.setColor(sf::Color(255, 255, 255, 128));

            window.draw(sprite);
        }
    }
}
