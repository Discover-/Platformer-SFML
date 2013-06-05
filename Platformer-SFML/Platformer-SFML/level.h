#ifndef __LEVEL_H
#define __LEVEL_H

#include <vector>
#include <SFML/Graphics.hpp>

class Game;

class Level
{
    public:
        Level(Game* _game);
        ~Level();

        int Update();
        void LoadMap(const char* filename);
        void DrawMap(sf::RenderWindow &window);

    private:
        Game* game;
        std::vector<std::vector<int>> mapVector;
        std::vector<char* /* filename */> fileVector;
        std::string fileArray[50][50];
};

#endif
