class Player;
class Game
{
    public:
        Game();
        ~Game();

        int Update();
        bool IsRunning() { return isRunning; }

    private:
        bool isRunning;
        Player* player;
};
