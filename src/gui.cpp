#include "gui.h"
#include <iostream>
#include <string>

GUI::GUI(int rows, int columns, int cellSize)
    : m_rows(rows)
    , m_columns(columns)
    , m_cellSize(cellSize)
    , m_cellMargin(cellSize / 20)
{
    // Initialize SDL library
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed:\n";
        std::cerr << SDL_GetError() << "\n";
    }

    // Create window
    int width = m_columns * m_cellSize;
    int height = m_rows * m_cellSize;
    m_window = SDL_CreateWindow("Connect Four", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

    // Create renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_SOFTWARE);
}

GUI::~GUI()
{
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void GUI::showPickColorScreen()
{
    // Set title
    std::string title = "Pick a color (yellow starts)";
    SDL_SetWindowTitle(m_window, title.c_str());

    // Paint screen red
    SDL_SetRenderDrawColor(m_renderer, s_rRed, s_gRed, s_bRed, s_alpha);
    SDL_RenderClear(m_renderer);

    // Paint left half of screen yellow
    SDL_Rect yellowOption;
    yellowOption.x = 0;
    yellowOption.y = 0;
    yellowOption.w = (m_columns * m_cellSize) / 2;
    yellowOption.h = m_rows * m_cellSize;
    SDL_SetRenderDrawColor(
        m_renderer, s_rYellow, s_gYellow, s_bYellow, s_alpha);
    SDL_RenderFillRect(m_renderer, &yellowOption);

    // Render
    SDL_RenderPresent(m_renderer);
}

void GUI::showGrid(const Grid& grid)
{
    // Clear screen
    SDL_SetRenderDrawColor(
        m_renderer, s_rBackground, s_gBackground, s_bBackground, s_alpha);
    SDL_RenderClear(m_renderer);

    // Render grid
    SDL_Rect cell;
    cell.w = m_cellSize - 2 * m_cellMargin;
    cell.h = m_cellSize - 2 * m_cellMargin;

    for (int row = 0; row < m_rows; ++row) {
        for (int column = 0; column < m_columns; ++column) {
            cell.x = column * m_cellSize + m_cellMargin;
            cell.y = row * m_cellSize + m_cellMargin;
            switch (grid.getCell(row, column)) {
            case Cell::YellowDisc:
                SDL_SetRenderDrawColor(
                    m_renderer, s_rYellow, s_gYellow, s_bYellow, s_alpha);
                break;
            case Cell::RedDisc:
                SDL_SetRenderDrawColor(
                    m_renderer, s_rRed, s_gRed, s_bRed, s_alpha);
                break;
            case Cell::Empty:
                SDL_SetRenderDrawColor(
                    m_renderer, s_rEmpty, s_gEmpty, s_bEmpty, s_alpha);
            }
            SDL_RenderFillRect(m_renderer, &cell);
        }
    }

    SDL_RenderPresent(m_renderer);
}

void GUI::updateTitle(State state, const Player& player)
{
    std::string title;
    std::string color = player.color == Color::Yellow ? "Yellow" : "Red";
    switch (state) {
    case State::Playing:
        title = color + "'s turn.";
        break;
    case State::Win:
        title = color + " wins!";
        break;
    case State::Draw:
        title = "It's a draw!";
    }
    SDL_SetWindowTitle(m_window, title.c_str());
}

bool GUI::pickColor(Player& yellowPlayer, Player& redPlayer)
{
    SDL_Event event;
    while (true) {
        if (!SDL_WaitEvent(&event)) {
            return false;
        }
        switch (event.type) {
        case SDL_QUIT:
            return false;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.x < (m_columns * m_cellSize) / 2) {
                yellowPlayer.human = true;
            } else {
                redPlayer.human = true;
            }
            return true;
        }
    }
    return false;
}

bool GUI::pickAction(int& action)
{
    SDL_Event event;
    while (true) {
        if (!SDL_WaitEvent(&event)) {
            return false;
        }
        switch (event.type) {
        case SDL_QUIT:
            return false;
        case SDL_MOUSEBUTTONDOWN:
            action = event.button.x / m_cellSize;
            return true;
        }
    }
    return false;
}

void GUI::waitForQuit()
{
    SDL_Event event;
    while (true) {
        if (!SDL_WaitEvent(&event)) {
            return;
        }
        if (event.type == SDL_QUIT) {
            return;
        }
    }
}