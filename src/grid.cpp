#include "grid.h"
#include <algorithm>

Grid::Grid(int rows, int columns, int toWin)
    : m_rows(rows)
    , m_columns(columns)
    , m_toWin(toWin)
    , m_cells(rows, std::vector<Cell>(columns, Cell::Empty))
{
}

Cell Grid::getCell(int row, int column) const { return m_cells[row][column]; }

bool Grid::place(int column, Color color)
{
    int row = -1;
    while (row + 1 < m_rows && m_cells[row + 1][column] == Cell::Empty) {
        ++row;
    }
    if (row == -1) {
        return false;
    }
    m_cells[row][column]
        = color == Color::Red ? Cell::RedDisc : Cell::YellowDisc;
    return true;
}

bool Grid::hasFourConnected() const
{
    // check rows
    for (int row = 0; row < m_rows; ++row) {
        for (int startCol = 0; startCol < m_columns; ++startCol) {
            if (m_cells[row][startCol] == Cell::Empty) {
                continue;
            }
            int length = 1;
            while (startCol + length < m_columns
                && m_cells[row][startCol + length] == m_cells[row][startCol]) {
                ++length;
            }
            if (length >= m_toWin) {
                return true;
            }
            startCol += length - 1;
        }
    }

    // check columns
    for (int col = 0; col < m_columns; ++col) {
        for (int startRow = 0; startRow < m_rows; ++startRow) {
            if (m_cells[startRow][col] == Cell::Empty) {
                continue;
            }
            int length = 1;
            while (startRow + length < m_rows
                && m_cells[startRow + length][col] == m_cells[startRow][col]) {
                ++length;
            }
            if (length >= m_toWin) {
                return true;
            }
            startRow += length - 1;
        }
    }

    // check tl - br diagonal (partitioned by row - col)
    for (int diff = -m_columns + 1; diff < m_rows; ++diff) {
        for (int startRow = std::max(0, diff);
             startRow < std::min(m_rows, m_columns + diff); ++startRow) {
            int startCol = startRow - diff;
            if (m_cells[startRow][startCol] == Cell::Empty) {
                continue;
            }
            int length = 1;
            while (startRow + length < m_rows && startCol + length < m_columns
                && m_cells[startRow + length][startCol + length]
                    == m_cells[startRow][startCol]) {
                ++length;
            }
            if (length >= m_toWin) {
                return true;
            }
            startRow += length - 1;
        }
    }

    // check tr - bl diagonal (partitioned by row + col)
    for (int sum = 0; sum < m_rows + m_columns - 1; ++sum) {
        for (int startRow = std::max(0, sum - m_columns + 1);
             startRow < std::min(m_rows, sum + 1); ++startRow) {
            int startCol = sum - startRow;
            if (m_cells[startRow][startCol] == Cell::Empty) {
                continue;
            }
            int length = 1;
            while (startRow + length < m_rows && startCol - length >= 0
                && m_cells[startRow + length][startCol - length]
                    == m_cells[startRow][startCol]) {
                ++length;
            }
            if (length >= m_toWin) {
                return true;
            }
            startRow += length - 1;
        }
    }
    return false;
}

bool Grid::isFull() const
{
    return std::find(
               m_cells.front().begin(), m_cells.front().end(), Cell::Empty)
        == m_cells.front().end();
}

std::vector<int> Grid::legalActions() const
{
    std::vector<int> legalActions;
    for (int i = 0; i < m_columns; ++i) {
        if (m_cells.front().at(i) == Cell::Empty) {
            legalActions.push_back(i);
        }
    }
    return legalActions;
}