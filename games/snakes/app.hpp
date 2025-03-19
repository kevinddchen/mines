#pragma once

#include <games/snakes/board.hpp>

#include <games/common/border.hpp>


namespace games::snakes
{

/**
 * Snakes application. Manages user input and game loop.
 */
class App
{
public:
    // Frame rate
    static constexpr double FRAMES_PER_SEC = 30.0;
    // Tick rate
    static constexpr double TICKS_PER_SEC = 1.0;

    // Top margin, in number of chars
    static constexpr int MARGIN_TOP = 1;
    // Left margin, in number of chars
    static constexpr int MARGIN_LEFT = 1;

    /**
     * Create application.
     * @param rows Number of rows for the Snakes board.
     * @param cols Number of columns for the Snakes board.
     */
    App(int rows, int cols);

    /**
     * Run the application.
     */
    void run();

private:
    /**
     * Refresh the windows of the application.
     */
    void refresh() const;

    /**
     * Perform action associated with given keystroke or mouse event.
     * @param key Key pressed.
     * @returns False when we want to quit.
     */
    bool handle_keystroke(int key);

    Border board_border;
    Board board;
};

}  // namespace games::snakes
