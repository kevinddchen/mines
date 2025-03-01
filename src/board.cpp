#include <optional>

#include "common.h"

#include "board.h"


namespace mines
{

Board::Board(int rows, int cols, int mines, int start_y, int start_x)
    : Component(newwin(rows + 2 * BORDER_WIDTH, cols + 2 * BORDER_WIDTH, start_y, start_x)),
      rows(rows),
      cols(cols),
      mines(mines),
      game(rows, cols, mines),
      state(BoardState::active),
      num_opened(0),
      num_flags(0),
      last_opened(std::nullopt)
{
    // create window border
    box(window, 0, 0);

    // create empty data structures
    is_known_mine_array.reserve(rows);
    is_opened_array.reserve(rows);
    is_flagged_array.reserve(rows);
    neighbor_mine_counts.reserve(rows);
    for (int i = 0; i < rows; ++i) {
        is_known_mine_array.emplace_back(cols, false);
        is_opened_array.emplace_back(cols, false);
        is_flagged_array.emplace_back(cols, false);
        neighbor_mine_counts.emplace_back(cols, -1);  // sentinel value for unset value
    }
}

int Board::click_cell(int row, int col)
{
    if (!is_active()) {
        return 1;
    } else if (is_flagged(row, col)) {
        return 3;
    } else if (!is_opened(row, col)) {
        open(row, col);
    } else if (can_chord(row, col)) {
        open_neighbors(row, col);
    } else {
        return 2;
    }
    return 0;
}

void Board::open(int row, int col)
{
    assert(can_open(row, col));

    // interact with backend
    std::optional<int> neighbor_mine_count = std::nullopt;  // this is set if `is_mine` is false
    const bool is_mine = game.open(row, col, neighbor_mine_count);

    // update state
    is_opened_array[row][col] = true;
    ++num_opened;
    last_opened = {row, col};

    // check if lost
    if (is_mine) {
        state = BoardState::lose;
        populate_known_mine_array();
        return;
    }

    neighbor_mine_counts[row][col] = neighbor_mine_count.value();

    // check if won
    if (num_opened + mines == rows * cols) {  // if all non-mine cells have been opened
        state = BoardState::win;
        populate_known_mine_array();
        return;
    }

    // if no neighboring mines, recursively open all neighboring cells
    if (neighbor_mine_count.value() == 0) {
        open_neighbors(row, col);
    }
}

void Board::open_neighbors(int row, int col)
{
    for (int dy : {-1, 0, 1}) {
        for (int dx : {-1, 0, 1}) {
            // skip case where not actually neighbor
            if (dy == 0 && dx == 0) {
                continue;
            }
            const int nb_row = row + dy;
            const int nb_col = col + dx;
            // check bounds
            if (nb_row < 0 || nb_row >= rows || nb_col < 0 || nb_col >= cols) {
                continue;
            }
            if (can_open(nb_row, nb_col)) {
                open(nb_row, nb_col);
            }
        }
    }
}

int Board::toggle_flag(int row, int col)
{
    if (!is_active()) {
        return 1;
    } else if (is_opened(row, col)) {
        return 2;
    }

    if (is_flagged_array[row][col]) {
        is_flagged_array[row][col] = false;
        --num_flags;
    } else {
        is_flagged_array[row][col] = true;
        ++num_flags;
    }
    return 0;
}

int Board::get_neighbor_flag_count(int row, int col) const
{
    int count = 0;
    for (int dy : {-1, 0, 1}) {
        for (int dx : {-1, 0, 1}) {
            // skip case where not actually neighbor
            if (dy == 0 && dx == 0) {
                continue;
            }
            const int nb_row = row + dy;
            const int nb_col = col + dx;
            // check bounds
            if (nb_row < 0 || nb_row >= rows || nb_col < 0 || nb_col >= cols) {
                continue;
            }
            if (is_flagged(nb_row, nb_col)) {
                ++count;
            }
        }
    }
    return count;
}

void Board::populate_known_mine_array()
{
    assert(!is_active());
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            is_known_mine_array[row][col] = game.is_mine(row, col);
        }
    }
}

void Board::refresh() const
{
    for (int row = 0; row < rows; ++row) {
        wmove(window, row + BORDER_WIDTH, BORDER_WIDTH);
        for (int col = 0; col < cols; ++col) {
            print_cell(row, col);
        }
    }
    wrefresh(window);
}

void Board::print_cell(int row, int col) const
{
    if (is_flagged(row, col)) {
        auto attr = A_BOLD;
        // if game ended and flag is incorrect, use red background and blink
        if (!is_active() && !is_known_mine(row, col)) {
            attr |= A_BLINK | COLOR_PAIR(COLOR_PAIR_ERROR);
        }
        wattron(window, attr);
        waddch(window, 'F');
        wattroff(window, attr);
        return;
    } else if (is_known_mine(row, col)) {
        auto attr = A_BOLD;
        // if last click, use red background and blink
        if (const auto& lo = last_opened.value(); lo.first == row && lo.second == col) {
            attr |= A_BLINK | COLOR_PAIR(COLOR_PAIR_ERROR);
        }
        wattron(window, attr);
        waddch(window, '*');
        wattroff(window, attr);
        return;
    } else if (!is_opened(row, col)) {
        const auto attr = COLOR_PAIR(COLOR_PAIR_UNOPENED);
        wattron(window, attr);
        waddch(window, '#');
        wattroff(window, attr);
        return;
    }
    // otherwise, empty cell. print number of neighboring mines
    const int neighbor_mines = get_neighbor_mine_count(row, col);
    if (neighbor_mines == 0) {
        waddch(window, ' ');
        return;
    } else {
        const char digit = static_cast<char>(neighbor_mines) + '0';
        const auto attr = COLOR_PAIR(neighbor_mines);
        wattron(window, attr);
        waddch(window, digit);
        wattroff(window, attr);
        return;
    }
}

}  // namespace mines
