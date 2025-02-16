#pragma once

#include <vector>

namespace mines
{

using cell_t = uint8_t;
using cells_t = std::vector<std::vector<cell_t>>;

// Binary flag indicating cell contains a mine.
constexpr cell_t MINE = 1;
// Binary flag indicating cell has been opened, i.e. is revealed to the user.
constexpr cell_t OPEN = 2;
// Binary flag indicating cell has been flagged.
constexpr cell_t FLAG = 4;

class Board
{
public:
    /**
     * Minesweeper board.
     * @param rows Number of rows.
     * @param cols Number of columns.
     * @param mines Number of mines.
     */
    Board(int rows, int cols, int mines);

    /**
     * Get state of all cells.
     */
    inline const cells_t& getCells() const { return cells; }

    /**
     * Get state of a cell.
     */
    inline cell_t getCell(int row, int col) const { return cells[row][col]; }

    /**
     * Open a cell.
     *
     * Return values:
     *   0 - success.
     *   1 - failure; game is inactive.
     *   2 - failure; cell has already been opened.
     *   3 - failure; cell has been flagged.
     */
    int open(int row, int col);

    /**
     * Toggle the flag for a cell.
     *
     * Return values:
     *   0 - success.
     *   1 - failure; game is inactive.
     *   2 - failure; cell has already been opened.
     */
    int toggleFlag(int row, int col);

    /**
     * Returns true if game is active.
     */
    inline bool isActive() const { return active; }

    /**
     * Print board state to console.
     */
    void print() const;

    const int rows;
    const int cols;
    const int mines;

private:
    // Whether game is active.
    bool active;
    // Array with shape (h, w) representing the state of all cells.
    cells_t cells;
};

/**
 * Returns true if cell contains a mine.
 */
inline bool isMine(cell_t cell_state)
{
    return cell_state & MINE;
}

/**
 * Returns true if cell has been opened.
 */
inline bool isOpened(cell_t cell_state)
{
    return cell_state & OPEN;
}

/**
 * Returns true if cell has been flagged.
 */
inline bool isFlagged(cell_t cell_state)
{
    return cell_state & FLAG;
}

}  // namespace mines
