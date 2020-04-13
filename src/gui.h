#ifndef GUI_H
#define GUI_H

#include "SDL.h"
#include "game.h"
#include <grid.h>

// Forward declarations
struct Player;
class Grid;
enum class State;

// Manages the Graphical User Interface
class GUI {
public:
    GUI(int rows, int columns, int cellSize);
    ~GUI();

    // Show the initial screen that let's the user pick a color
    void showPickColorScreen();

    // Show the grid during play
    void showGrid(const Grid& grid);

    // Update the title according to the current player and the game state
    void updateTitle(State state, const Player& player);

    // Let the user pick a color, false iff user quits game
    bool pickColor(Player& yellowPlayer, Player& redPlayer);

    // Let the user pick an action, false iff user quits game
    bool pickAction(int& action);

    // Wait until user quits game
    void waitForQuit();

private:
    // Background color
    static constexpr Uint8 s_rBackground = 50;
    static constexpr Uint8 s_gBackground = 70;
    static constexpr Uint8 s_bBackground = 150;

    // Empty cell color
    static constexpr Uint8 s_rEmpty = 30;
    static constexpr Uint8 s_gEmpty = 40;
    static constexpr Uint8 s_bEmpty = 90;

    // Yellow disc color
    static constexpr Uint8 s_rYellow = 250;
    static constexpr Uint8 s_gYellow = 200;
    static constexpr Uint8 s_bYellow = 10;

    // Red disc color
    static constexpr Uint8 s_rRed = 250;
    static constexpr Uint8 s_gRed = 10;
    static constexpr Uint8 s_bRed = 10;

    // Max alpha for no opacity
    static constexpr Uint8 s_alpha = 255;

    // Screen size parameters
    const int m_rows;
    const int m_columns;
    const int m_cellSize;
    const int m_cellMargin;

    // SDL handles
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
};

#endif