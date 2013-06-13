#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "shareddefines.h"

class Player;
class Bullet;
class Level;
class Enemy;
class Tile;
class Coin;
class Unit;
class MenuPlayer;

class Game
{
    public:
        Game();
        ~Game();

        int Update();
        bool IsRunning() { return isRunning; }
        void StartActualGame(sf::RenderWindow &window, std::string filename);

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
        void SetPlayer(Player* _player) { player = _player; RemoveUnitWithTypeId(TYPEID_PLAYER); if (_player) allUnits.push_back((Unit*)_player); }
        MenuPlayer* GetMenuPlayer() { return menuPlayer; }

        void AddBullet(Bullet* bullet) { allBullets.push_back(bullet); }
        std::vector<Bullet*> &GetBullets() { return allBullets; }

        GameState GetGameState() { return gameState; }
        void SetGameState(GameState state) { gameState = state; }

        std::vector<Enemy*> &GetEnemies() { return allEnemies; }
        void ClearAllEnemies() { allEnemies.clear(); RemoveUnitWithTypeId(TYPEID_ENEMY); }
        void AddEnemy(Enemy* enemy) { allEnemies.push_back(enemy); allUnits.push_back((Unit*)enemy); }

        void RemoveUnitWithTypeId(UnitTypeId typeId);

        std::vector<Tile*> &GetTiles() { return allTiles; }
        void ClearAllTiles() { allTiles.clear(); }
        void AddTile(Tile* tile) { allTiles.push_back(tile); }

        std::vector<Coin*> &GetCoins() { return allCoins; }
        void ClearCoins() { allCoins.clear(); }
        void AddCoin(Coin* coin) { allCoins.push_back(coin); }

        bool IsQuickSandArea(float x, float y, float h, float w);
        bool IsInWaterArea(float x, float y, float h, float w);
        bool IsInLavaArea(float x, float y, float h, float w);

    private:
        bool isRunning, showDebugInfo;
        Player* player;
        MenuPlayer* menuPlayer;
        std::vector<sf::Sprite> gameObjects;
        std::vector<sf::Sprite> gameObjectsCollidable;
        std::vector<sf::Sprite> quickSandGameobjects;
        std::vector<sf::Sprite> waterGameobjects;
        std::vector<sf::Sprite> lavaGameobjects;
        std::vector<Bullet*> allBullets;
        std::vector<Enemy*> allEnemies;
        std::vector<Unit*> allUnits;
        GameState gameState;
        Level* currLevel;
        std::vector<Tile*> allTiles;
        std::vector<Coin*> allCoins;
};
