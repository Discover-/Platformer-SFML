#include <vector>
#include <SFML/Graphics.hpp>

class Player;

class Game
{
    public:
        Game();
        ~Game();

        int Update();
        bool IsRunning() { return isRunning; }
        std::vector<sf::Sprite> GetGameObjects() { return gameObjects; }
        std::vector<sf::Sprite> GetGameObjectsCollidable() { return gameObjectsCollidable; }

    private:
        bool isRunning;
        Player* player;
        std::vector<sf::Sprite> gameObjects;
        std::vector<sf::Sprite> gameObjectsCollidable;
};
