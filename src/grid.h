#ifndef GRID_H
#define GRID_H

#include "game.h"
#include <vector>

// Forward declaration
enum class Color;

// A cell can be either empty or contain a disc
enum class Cell { YellowDisc, RedDisc, Empty };

// Manages the game grid
class Grid {
public:
    Grid(int rows, int columns, int toWin);

    // Get the cell in the corresponding row and column
    Cell getCell(int row, int column) const;

    // Try to place a disc of the given color into the given column
    bool place(int column, Color color);

    // Check if no empty cells are left
    bool isFull() const;

    // Check if there are m_toWin discs of the same color in a row
    bool hasFourConnected() const;

    // Get a vector containing all columns into which a disc can be placed
    std::vector<int> legalActions() const;

private:
    int m_rows;
    int m_columns;
    int m_toWin;
    std::vector<std::vector<Cell>> m_cells;
};

#endif