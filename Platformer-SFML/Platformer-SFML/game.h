#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "shareddefines.h"
#include "movingtile.h"

class Player;
class Bullet;
class Level;
class Enemy;

class Game
{
    public:
        Game();
        ~Game();

        int Update();
        bool IsRunning() { return isRunning; }
        void StartActualGame(sf::RenderWindow &window);

        std::vector<sf::Sprite> GetGameObjects() { return gameObjects; }
        std::vector<sf::Sprite> GetGameObjectsCollidable() { return gameObjectsCollidable; }
        void AddGameObject(sf::Sprite gameobject) { gameObjects.push_back(gameobject); }
        void AddGameObjectCollidable(sf::Sprite gameobject) { gameObjectsCollidable.push_back(gameobject); }
        void ClearGameObjects() { gameObjects.clear(); }
        void ClearGameObjectCollidables() { gameObjectsCollidable.clear(); }

        void HandleTimers(sf::Int32 diff_time);

        Player* GetPlayer() { return player; }

        void AddBullet(Bullet* bullet) { allBullets.push_back(bullet); }
        std::vector<Bullet*> GetBullets() { return allBullets; }

        GameState GetGameState() { return STATE_PLAYING; }
        void SetGameState(GameState state) { gameState = state; }

        std::vector<Enemy*> GetEnemies() { return allEnemies; }
        std::vector<MovingTile*> GetMovingTiles() { return movingTiles; }

    private:
        bool isRunning;
        Player* player;
        std::vector<sf::Sprite> gameObjects;
        std::vector<sf::Sprite> gameObjectsCollidable;
        std::vector<Bullet*> allBullets;
        std::vector<Enemy*> allEnemies;
        GameState gameState;
        Level* currLevel;
        std::vector<MovingTile*> movingTiles;
};


