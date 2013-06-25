#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "shareddefines.h"

class Player;
class Bullet;
class Level;
class Enemy;
class SpecialTile;
class Coin;
class Unit;
class MenuPlayer;
class Audio;

class Game
{
    public:
        static Game& instance()
        {
            static Game game;
            return game;
        }

        int Update();
        bool IsRunning() { return isRunning; }
        void StartActualGame(sf::RenderWindow &window, std::string filename);

        void DeleteContentMemory();

        void LoadAllAudio();

        GameState GetGameState() { return gameState; }
        void SetGameState(GameState state) { gameState = state; }
        void SetUpcomingGameState(GameState state) { upcomingGameState = state; }

        std::vector<sf::Sprite> &GetGameObjects() { return gameObjects; }
        std::vector<sf::Sprite> &GetGameObjectsCollidable() { return gameObjectsCollidable; }
        void AddGameObject(sf::Sprite gameobject) { gameObjects.push_back(gameobject); }
        void AddGameObjectCollidable(sf::Sprite gameobject) { gameObjectsCollidable.push_back(gameobject); }
        void ClearGameObjects() { gameObjects.clear(); }
        void ClearGameObjectCollidables() { gameObjectsCollidable.clear(); }

        void HandleTimers(sf::Int32 diff_time);

        void AddUnit(Unit* unit) { allUnits.push_back(unit); }

        Player* GetPlayer() { return player; }
        void SetPlayer(Player* _player) { player = _player; RemoveUnitWithTypeId(TYPEID_PLAYER); if (_player) allUnits.push_back((Unit*)_player); }
        MenuPlayer* GetMenuPlayer() { return menuPlayer; }
        void SetMenuPlayer(MenuPlayer* _menuPlayer) { menuPlayer = _menuPlayer; RemoveUnitWithTypeId(TYPEID_MENU_PLAYER); if (_menuPlayer) allUnits.push_back((Unit*)_menuPlayer); }

        std::vector<Enemy*> &GetEnemies() { return allEnemies; }
        void ClearAllEnemies() { allEnemies.clear(); RemoveUnitWithTypeId(TYPEID_ENEMY); }
        void AddEnemy(Enemy* enemy) { allEnemies.push_back(enemy); allUnits.push_back((Unit*)enemy); }

        void AddBullet(Bullet* bullet) { allBullets.push_back(bullet); }
        void RemoveBullet(Bullet* bullet);
        std::vector<Bullet*> &GetBullets() { return allBullets; }

        void RemoveUnitWithTypeId(UnitTypeId typeId);

        std::vector<SpecialTile*> &GetSpecialTiles() { return allSpecialTiles; }
        void ClearAllSpecialTiles() { allSpecialTiles.clear(); }
        void AddSpecialTile(SpecialTile* tile) { allSpecialTiles.push_back(tile); }

        std::vector<Coin*> &GetCoins() { return allCoins; }
        void ClearCoins() { allCoins.clear(); }
        void AddCoin(Coin* coin) { allCoins.push_back(coin); }

        bool IsMusicMuted() { return mutedMusic; }

        void LoadedAnotherTile() { loadedTiles++; }
        void SetLoadedTiles(int val) { loadedTiles = val; }

        void SetCurrentlyLoadingLvl(std::string val) { currentlyLoadingLvl = val; }

        Audio* GetAudio() { return audio; }

    private:
        Game();
        ~Game();

        bool isRunning, showDebugInfo, mutedMusic;
        Player* player;
        MenuPlayer* menuPlayer;
        Audio* audio;
        std::vector<sf::Sprite> gameObjects;
        std::vector<sf::Sprite> gameObjectsCollidable;
        std::vector<Bullet*> allBullets;
        std::vector<Enemy*> allEnemies;
        std::vector<Unit*> allUnits;
        GameState gameState, upcomingGameState;
        Level* currLevel;
        std::vector<SpecialTile*> allSpecialTiles;
        std::vector<Coin*> allCoins;
        int loadedTiles;
        std::string currentlyLoadingLvl;
};

#define sGame Game::instance()
