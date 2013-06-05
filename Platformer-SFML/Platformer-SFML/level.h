#ifndef __LEVEL_H
#define __LEVEL_H

#include <vector>
#include <array>
#include <SFML/Graphics.hpp>

//struct TileInfo
//{
//    int layer;
//    std::string fileName; // maken we op uit line[i] want line[i] is een nummer wat de tegel aangeeft
//    sf::Sprite sprite;
//};

class Game;

class Level
{
    public:
        Level(Game* _game);
        ~Level();

        int Update();
        void LoadMap(char const* filename, sf::RenderWindow &window);
        void DrawMap(sf::RenderWindow &window);

    private:
        Game* game;
        std::vector<sf::Sprite> mapVector;
        std::vector<char* /* filename */> fileVector;
        //sf::Sprite* map[1000][1000];
};

#endif
