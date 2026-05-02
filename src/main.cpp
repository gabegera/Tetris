#include "Game.h"

int main()
{
    Game game;
    game.Start();

    while (game.IsRunning())
    {
        game.Update();
    }
}
