#include <ngames/snake/app.hpp>

#include <ngames/common/ncurses.hpp>

#include <cstdlib>


int main()
{
    ngames::init_ncurses();

    ngames::snake::App app(15, 21, 5.0);
    app.run();

    ngames::end_ncurses();
    return EXIT_SUCCESS;
}
