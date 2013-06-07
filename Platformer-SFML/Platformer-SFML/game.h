#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "shareddefines.h"

class Player;
class Bullet;
class Level;
class Enemy;
class MovingTile;
class Coin;

class Game
{
    public:
        Game();
        ~Game();

        int Update();
        bool IsRunning() { return isRunning; }
        void StartActualGame(sf::RenderWindow &window);

        std::vector<sf::Sprite> &GetGameObjects() { return gameObjects; }
        std::vector<sf::Sprite> &GetGameObjectsCollidable() { return gameObjectsCollidable; }
        std::vector<sf::Sprite> &GetQuickSandObjects() { return quickSandGameobjects; }
        std::vector<sf::Sprite> &GetWaterObjects() { return waterGameobjects; }
        std::vector<sf::Sprite> &GetLavaObjects() { return lavaGameobjects; }
        void AddGameObject(sf::Sprite gameobject) { gameObjects.push_back(gameobject); }
        void AddGameObjectCollidable(sf::Sprite gameobject) { gameObjectsCollidable.push_back(gameobject); }
        void AddQuickSandObject(sf::Sprite quickSandObject) { quickSandGameobjects.push_back(quickSandObject); }
        void AddWaterObject(sf::Sprite waterObject) { waterGameobjects.push_back(waterObject); }
        void AddLavaObject(sf::Sprite lavaObject) { lavaGameobjects.push_back(lavaObject); }
        void ClearGameObjects() { gameObjects.clear(); }
        void ClearGameObjectCollidables() { gameObjectsCollidable.clear(); }
        void ClearQuickSandObjects() { quickSandGameobjects.clear(); }
        void ClearWaterObjects() { waterGameobjects.clear(); }
        void ClearLavaObjects() { lavaGameobjects.clear(); }

        void HandleTimers(sf::Int32 diff_time);

        Player* GetPlayer() { return player; }

        void AddBullet(Bullet* bullet) { allBullets.push_back(bullet); }
        std::vector<Bullet*> &GetBullets() { return allBullets; }

        GameState GetGameState() { return gameState; }
        void SetGameState(GameState state) { gameState = state; }

        std::vector<Enemy*> &GetEnemies() { return allEnemies; }
        std::vector<MovingTile*> &GetMovingTiles() { return movingTiles; }
        void ClearMovingTiles() { movingTiles.clear(); }
        void AddMovingTile(MovingTile* movingTile) { movingTiles.push_back(movingTile); }
        std::vector<Coin*> &GetCoins() { return allCoins; }
        void ClearCoins() { allCoins.clear(); }
        void AddCoin(Coin* coin) { allCoins.push_back(coin); }

        bool IsQuickSandArea(float x, float y, float h, float w);
        bool IsInWaterArea(float x, float y, float h, float w);
        bool IsInLavaArea(float x, float y, float h, float w);

    private:
        bool isRunning;
        Player* player;
        std::vector<sf::Sprite> gameObjects;
        std::vector<sf::Sprite> gameObjectsCollidable;
        std::vector<sf::Sprite> quickSandGameobjects;
        std::vector<sf::Sprite> waterGameobjects;
        std::vector<sf::Sprite> lavaGameobjects;
        std::vector<Bullet*> allBullets;
        std::vector<Enemy*> allEnemies;
        GameState gameState;
        Level* currLevel;
        std::vector<MovingTile*> movingTiles;
        std::vector<Coin*> allCoins;
};

