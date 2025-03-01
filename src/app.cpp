#include <ncurses.h>

#include "common.h"

#include "app.h"


namespace mines
{

App::App(int rows, int cols, int mines)
    : cursor_y((rows - 1) / 2),
      cursor_x((cols - 1) / 2),
      text_mine_count(board, MARGIN_TOP, MARGIN_LEFT),                              // height is 1
      board(rows, cols, mines, MARGIN_TOP + 1, MARGIN_LEFT),                        // height is rows + 2 * BORDER_WIDTH
      text_end_game(board, MARGIN_TOP + 1 + rows + 2 * BORDER_WIDTH, MARGIN_LEFT),  // height is 1
      text_instructions(MARGIN_TOP + 2 + rows + 2 * BORDER_WIDTH, MARGIN_LEFT)
{
}

void App::run()
{
    // Allow arrow keys
    keypad(board.window, true);

    // Initial print
    text_mine_count.refresh();
    board.refresh();
    text_end_game.refresh();
    text_instructions.refresh();

    while (true) {
        wmove(board.window, cursor_y + BORDER_WIDTH, cursor_x + BORDER_WIDTH);
        const int key = wgetch(board.window);
        const bool continue_ = handle_keystroke(key);
        if (!continue_) {
            break;
        }
    }

    endwin();
}

bool App::handle_keystroke(int key)
{
    switch (key) {
        case 'h':
        case KEY_LEFT:
            if (cursor_x > 0) {
                --cursor_x;
            }
            break;
        case 'j':
        case KEY_DOWN:
            if (cursor_y < board.rows - 1) {
                ++cursor_y;
            }
            break;
        case 'k':
        case KEY_UP:
            if (cursor_y > 0) {
                --cursor_y;
            }
            break;
        case 'l':
        case KEY_RIGHT:
            if (cursor_x < board.cols - 1) {
                ++cursor_x;
            }
            break;
        case 'f':
            board.toggle_flag(cursor_y, cursor_x);
            text_mine_count.refresh();
            board.refresh();
            break;
        case ' ':
            board.click_cell(cursor_y, cursor_x);
            board.refresh();
            text_end_game.refresh();
            break;
        case 'q':
            return false;
    }
    return true;
}

}  // namespace mines
