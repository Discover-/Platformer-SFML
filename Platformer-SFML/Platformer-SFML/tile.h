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

class SpecialTile : public Position
{
    public:
        SpecialTile(sf::RenderWindow* _window, sf::Texture _image, sf::Vector2f position, TileTypeId _typeId);
        //~SpecialTile();

        virtual void Update();
        virtual void HandleTimers(sf::Int32 diff_time);
        virtual bool OnCollision(Unit* unit = NULL) = 0; //! Return true if we should stop movement
        virtual void OnCollisionOut(Unit* unit) = 0;

        bool IsRemoved() { return isRemoved; }
        void Draw(sf::Texture* _textureTil = NULL, bool updatePos = false);

        TileTypeId GetTypeId() { return typeId; }
        sf::Sprite GetSpriteTile();

    private:
        TileTypeId typeId;
        bool isRemoved;
        sf::Texture image;
        sf::RenderWindow* window;
};
