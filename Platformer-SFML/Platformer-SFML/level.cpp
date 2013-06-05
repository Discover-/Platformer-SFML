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

void Level::LoadMap(const char* filename)
{
    std::ifstream openfile(filename);
    std::string line;
    std::vector<int> tempVector;

    while (std::getline(openfile, line))
    {
        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] != ' ')
            {
                char value[1] = { line[i] };
                tempVector.push_back(atoi(value));
            }
        }

        mapVector.push_back(tempVector);
        tempVector.clear();
    }

    for (int i = 0; i < mapVector.size(); i++)
    {
        for (int j = 0; j < mapVector[i].size(); j++)
        {
            std::string randNumb = std::to_string(long double(urand(0, 1)));

            switch (mapVector[i][j])
            {
                case 9:
                    fileArray[i][j] = "Graphics/Tiles/sky_3.png"; //! Empty sky block
                    break;
                case 0:
                    fileArray[i][j] = "Graphics/Tiles/sky_" + std::to_string(long double(urand(0, 20) < 16 ? 3 : urand(0, 2))) + ".png";
                    break;
                case 1:
                    fileArray[i][j] = "Graphics/Tiles/dirt_" + randNumb + ".png";
                    break;
                case 2:
                    fileArray[i][j] = "Graphics/Tiles/dirt_rock_" + randNumb + ".png";
                    break;
                case 3:
                    fileArray[i][j] = "Graphics/Tiles/grass_" + randNumb + ".png";
                    break;
                case 4:
                    fileArray[i][j] = "Graphics/Tiles/grass_ontop_" + randNumb + ".png";
                    break;
                case 5:
                    fileArray[i][j] = "Graphics/Tiles/ground_" + randNumb + ".png";
                    break;
                case 6:
                    fileArray[i][j] = "Graphics/Tiles/sand_" + randNumb + ".png";
                    break;
                default:
                    return;
            }
        }
    }
}

void Level::DrawMap(sf::RenderWindow &window)
{
    sf::Sprite sprite;
    sf::Texture image;
    Player* player = game->GetPlayer();

    for (int i = 0; i < mapVector.size(); i++)
    {
        for (int j = 0; j < mapVector[i].size(); j++)
        {
            //! ONLY draw the images if the player is within visibility distance, else there's no point in wasting performance.
            if (IsInRange(player->GetPositionX(), j * 50.0f, player->GetPositionY(), i * 50.0f, 600.0f))
            {
                image.loadFromFile(fileArray[i][j]);
                sprite.setTexture(image);
                sprite.setPosition(j * 50.0f, i * 50.0f);
                window.draw(sprite);
            }
        }
    }
}
