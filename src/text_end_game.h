#pragma once

#include "board.h"
#include "ui.h"


namespace mines
{

/**
 * Text displaying end game message.
 */
class TextEndGame : public Component
{
public:
    /**
     * Create text.
     * @param board Reference to board object.
     * @param start_y y-coordinate of the top-left corner of the window.
     * @param start_x x-coordinate of the top-left corner of the window.
     */
    TextEndGame(const Board& board, int start_y, int start_x);

    /**
     * Refresh the text viewed by the user.
     */
    void refresh() const override;

private:
    const Board& board;
};

}  // namespace mines
