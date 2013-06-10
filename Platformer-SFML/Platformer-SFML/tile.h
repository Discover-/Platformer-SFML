#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <time.h>
#include <unordered_map>
#include <set>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "position.h"
#include "shareddefines.h"
#include "unit.h"

class Game;

class Tile : public Position
{
    public:
        Tile(Game* _game, sf::RenderWindow* _window, sf::Texture _image, sf::Vector2f position, TileTypeId _typeId);
        //~Tile();

        virtual void Update();
        virtual void HandleTimers(sf::Int32 diff_time);
        virtual bool OnCollision(Unit* unit = NULL) = 0; //! Return true if we should stop movement

        bool IsRemoved() { return isRemoved; }
        void Draw(sf::Texture* _textureTil = NULL, bool updatePos = false);

        TileTypeId GetTypeId() { return typeId; }
        sf::Sprite GetSpriteTile();
        Game* &GetGame() { return game; }

    private:
        Game* game;
        TileTypeId typeId;
        bool isRemoved;
        sf::Texture image;
        sf::RenderWindow* window;
};
