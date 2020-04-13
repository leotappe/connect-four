#include "game.h"
#include "ai.h"
#include "grid.h"
#include <iostream>

Player::Player(Color color)
    : color(color)
    , human(false)
{
}

Game::Game(int rows, int columns, int toWin, int monteCarloIterations)
    : m_rows(rows)
    , m_columns(columns)
    , m_toWin(toWin)
    , m_monteCarloIterations(monteCarloIterations)
{
}

void Game::run(GUI& gui)
{
    // Initialize players
    Player yellowPlayer(Color::Yellow);
    Player redPlayer(Color::Red);
    yellowPlayer.opponent = &redPlayer;
    redPlayer.opponent = &yellowPlayer;

    // Pick colors
    gui.showPickColorScreen();
    if (!gui.pickColor(yellowPlayer, redPlayer)) {
        return;
    }

    // Set up game
    AI ai(m_monteCarloIterations);
    Grid grid(m_rows, m_columns, m_toWin);
    State state = State::Playing;
    Player* currentPlayer = &yellowPlayer;

    // Show initial grid
    gui.showGrid(grid);
    gui.updateTitle(state, *currentPlayer);

    // Game loop
    while (true) {
        // Check if game is over
        if (state == State::Draw || state == State::Win) {
            gui.waitForQuit();
            break;
        }

        // Get move from either human or computer
        if (currentPlayer->human) {
            int action;
            while (true) {
                if (!gui.pickAction(action)) {
                    return;
                }
                if (grid.place(action, currentPlayer->color)) {
                    break;
                }
            }
        } else {
            grid.place(
                ai.getAction(grid, currentPlayer->color), currentPlayer->color);
        }

        // Update game state
        if (grid.hasFourConnected()) {
            state = State::Win;
        } else if (grid.isFull()) {
            state = State::Draw;
        } else {
            state = State::Playing;
            currentPlayer = currentPlayer->opponent;
        }

        // Update screen
        gui.showGrid(grid);
        gui.updateTitle(state, *currentPlayer);
    }
}
