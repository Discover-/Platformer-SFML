#include "game.h"

int main()
{
    Game* game = new Game();
    int result = game->Update();
    delete game;
    return result;
}
