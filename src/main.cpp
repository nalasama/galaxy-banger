#include "Game.h"

int SDL_main(int argc, char* argv[]) {
    Game game;
    if (!game.init()) return -1;

    while (game.isRunning()) {
        game.handleEvents();
        game.update();
        game.render();
        SDL_Delay(16);
    }

    return 0;
}
