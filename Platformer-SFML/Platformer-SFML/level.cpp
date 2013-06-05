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

void Level::LoadMap(char const* filename, sf::RenderWindow &window)
{
    std::ifstream openfile(filename);
    std::string line;
    std::vector<std::pair<sf::Image, sf::Sprite>> tempVector;
    //sf::Sprite* sprite;
    sf::Texture image;

    int layer = 0;

    std::cout << "Level 1 design: " << std::endl;

    while (std::getline(openfile, line))
    {
        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] != ' ')
            {
                std::cout << line[i] << " - ";
                std::string fileName = GetTileFilename(line[i]);

                image.loadFromFile(fileName);
                sf::Sprite sprite(image);
                sprite.setTexture(image);
                sprite.setPosition(layer * 50.0f, i * 50.0f);
            }
        }
    }
}

void Level::DrawMap(sf::RenderWindow &window)
{
    Player* player = game->GetPlayer();
    //window.draw(*map[5][5]);

    //for (int i = 0; i < 100; ++i)
    //    for (int j = 0; j < 100; ++j)
    //        if (sf::Sprite* sprite = map[i][j])
    //            window.draw(*sprite);

    //for (std::vector<sf::Sprite>::iterator itr = mapVector.begin(); itr != mapVector.end(); ++itr)
    {
        //! ONLY draw the images if the player is within visibility distance, else there's no point in wasting performance.
        //if (IsInRange(player->GetPositionX(), j * 50.0f, player->GetPositionY(), i * 50.0f, 600.0f))
        {
            //sprite.setPosition(j * 50.0f, i * 50.0f);
            //window.draw((*itr));
        }
    }

    //for (int i = 0; i < mapVector.size(); i++)
    //{
    //    for (int j = 0; j < mapVector[i].size(); j++)
    //    {
    //        //! ONLY draw the images if the player is within visibility distance, else there's no point in wasting performance.
    //        if (IsInRange(player->GetPositionX(), j * 50.0f, player->GetPositionY(), i * 50.0f, 600.0f))
    //        {
    //            image.loadFromFile(fileArray[i][j]);
    //            sprite.setTexture(image);
    //            sprite.setPosition(j * 50.0f, i * 50.0f);
    //            window.draw(sprite);
    //        }
    //    }
    //}
}
