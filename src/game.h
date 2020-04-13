#ifndef GAME_H
#define GAME_H

#include "gui.h"

// Forward declaration
class GUI;

// Two players
enum class Color { Yellow, Red };

// Three game states
enum class State { Playing, Win, Draw };

// Stores information about a player
struct Player {
    Player(Color color);
    Color color;
    bool human;
    Player* opponent;
};

// Manages the game
class Game {
public:
    Game(int rows, int columns, int toWin, int monteCarloIterations);

    // Run the game, i.e. pick a color and run main loop
    void run(GUI& gui);

private:
    const int m_rows;
    const int m_columns;
    const int m_toWin;
    const int m_monteCarloIterations;
};

#endif