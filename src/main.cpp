#include "game.h"
#include "gui.h"

int main()
{
    // Game parameters, change here if desired
    constexpr int rows = 6;
    constexpr int columns = 7;
    constexpr int toWin = 4;
    constexpr int cellSize = 100;

    // Make this larger to make the AI stronger (but a little slower).
    // The 15000 iterations version is probably a decent challenge.
    constexpr int monteCarloIterations = 5000;

    Game game(rows, columns, toWin, monteCarloIterations);
    GUI gui(rows, columns, cellSize);
    game.run(gui);
}