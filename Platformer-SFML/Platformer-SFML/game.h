#include <vector>
#include <SFML/Graphics.hpp>

class Player;
class Bullet;

class Game
{
    public:
        Game();
        ~Game();

        int Update();
        bool IsRunning() { return isRunning; }
        std::vector<sf::Sprite> GetGameObjects() { return gameObjects; }
        std::vector<sf::Sprite> GetGameObjectsCollidable() { return gameObjectsCollidable; }
        void AddGameObject(sf::Sprite gameobject) { gameObjects.push_back(gameobject); }
        void AddGameObjectCollidable(sf::Sprite gameobject) { gameObjectsCollidable.push_back(gameobject); }
        void HandleTimers(sf::Int32 diff_time);
        Player* GetPlayer() { return player; }
        void AddBullet(Bullet* bullet) { allBullets.push_back(bullet); }
        std::vector<Bullet*> GetBullets() { return allBullets; }

    private:
        bool isRunning;
        Player* player;
        std::vector<sf::Sprite> gameObjects;
        std::vector<sf::Sprite> gameObjectsCollidable;
        std::vector<Bullet*> allBullets;
};
