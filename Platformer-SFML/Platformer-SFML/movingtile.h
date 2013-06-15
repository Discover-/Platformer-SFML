#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <time.h>
#include <unordered_map>
#include <set>
#include <string>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include "unit.h"
#include "position.h"
#include "tile.h"

class Game;

class MovingTile : public SpecialTile
{
    public:
        MovingTile(Game* _game, sf::RenderWindow* _window, sf::Texture _image, int _velocity, sf::Vector2f startPosition, sf::Vector2f _destination, bool _movesVertical);
        ~MovingTile();

        void Update();
        void HandleTimers(sf::Int32 diff_time);
        bool OnCollision(Unit* unit = NULL); //! Return true if we should stop movement of player when colliding
        void OnCollisionOut(Unit* unit);

        void AddPassenger(Unit* unit);
        void RemovePassenger(Unit* unit);
        bool HasPassenger(Unit* unit);

    private:
        int velocity;
        bool movesVertical, movingToActualDest;
        std::list<Unit*> passengers;
        sf::Vector2f startPosition, destination;
};
