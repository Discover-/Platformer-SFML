

class Game
{
    public:
        Game();
        ~Game();

        int Update();
        bool IsRunning() { return isRunning; }

    private:
        bool isRunning;

        bool isJumping;
        bool isFalling;
        int fallSpeed;
        int jumpSpeed;
};
