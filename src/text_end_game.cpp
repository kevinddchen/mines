#include "common.h"

#include "text_end_game.h"


namespace mines
{

TextEndGame::TextEndGame(const Board& board, int start_y, int start_x)
    : Component(newwin(TextEndGame::HEIGHT, TextEndGame::WIDTH, start_y, start_x)),
      board(board)
{
}

void TextEndGame::refresh() const
{
    // check if game has ended
    if (!board.is_active()) {
        if (board.is_win()) {
            const auto attr = A_BOLD | COLOR_PAIR(COLOR_PAIR_WIN);
            wattron(window, attr);
            mvwprintw(window, 0, 0, "YOU HAVE WON!");
            wattroff(window, attr);
        } else {
            const auto attr = A_BOLD | COLOR_PAIR(COLOR_PAIR_LOSS);
            wattron(window, attr);
            mvwprintw(window, 0, 0, "YOU HAVE LOST...");
            wattroff(window, attr);
        }
    } else {
        wclear(window);
    }
    wrefresh(window);
}

}  // namespace mines
