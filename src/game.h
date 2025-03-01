#pragma once

#include <optional>
#include <vector>


namespace mines
{

/**
 * Back-end for the Minesweeper game. Contains secret game state hidden from
 * the player, i.e. the locations of all mines.
 */
class Game
{
public:
    /**
     * Create back-end for new Minesweeper game.
     * @param rows Number of rows.
     * @param cols Number of columns.
     * @param mines Number of mines.
     */
    Game(int rows, int cols, int mines);

    /**
     * Open a cell. First cell opened is guaranteed to not contain a mine.
     *
     * Throws an error if the game is not active or the cell has already been
     * opened.
     *
     * @param row Cell row.
     * @param col Cell column.
     * @param neighbor_mine_count If cell is not a mine, will be set to the
     * number of neighboring mines.
     *
     * @returns Whether the cell contains a mine.
     */
    bool open(int row, int col, std::optional<int>& neighbor_mine_count);

    /**
     * Returns true if cell contains a mine.
     *
     * Throws an error if the game is active.
     *
     * @param row Cell row.
     * @param col Cell column.
     */
    bool is_mine(int row, int col) const;

private:
    const int rows;
    const int cols;
    const int mines;

    // Whether the game is active.
    bool active;
    // Number of opened cells.
    int num_opened;

    // Array with shape (rows, cols) tracking which cells contain a mine.
    std::vector<std::vector<bool>> is_mine_array;
    // Array with shape (rows, cols) tracking which cells have been opened.
    std::vector<std::vector<bool>> is_opened_array;
};

}  // namespace mines
