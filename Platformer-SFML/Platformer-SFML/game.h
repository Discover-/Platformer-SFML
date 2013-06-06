#ifndef __GAME_H
#define __GAME_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "shareddefines.h"
#include "level.h"
#include "enemy.h"

class Player;
class Bullet;

class Game
{
    public:
        Game();
        ~Game();

        int Update();
        bool IsRunning() { return isRunning; }
        void HandleTimers(sf::Int32 diff_time);
        std::vector<sf::Sprite> GetGameObjects() { return gameObjects; }
        std::vector<sf::Sprite> GetGameObjectsCollidable() { return gameObjectsCollidable; }
        void AddGameObject(sf::Sprite gameobject) { gameObjects.push_back(gameobject); }
        void AddGameObjectCollidable(sf::Sprite gameobject) { gameObjectsCollidable.push_back(gameobject); }
        Player* GetPlayer() { return player; }
        void AddBullet(Bullet* bullet) { allBullets.push_back(bullet); }
        std::vector<Bullet*> GetBullets() { return allBullets; }
        void StartActualGame(sf::RenderWindow &window);

        GameState GetGameState() { return STATE_PLAYING; }
        void SetGameState(GameState state) { gameState = state; }

    private:
        bool isRunning;
        Player* player;
        std::vector<sf::Sprite> gameObjects;
        std::vector<sf::Sprite> gameObjectsCollidable;
        std::vector<Bullet*> allBullets;
        std::vector<Enemy*> allEnemies;
        GameState gameState;
        Level* currLevel;
};

#endif
