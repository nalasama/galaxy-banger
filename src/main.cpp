#include "Game.h"

int SDL_main(int argc, char* argv[]) {
    Game game;
    if (!game.init()) return -1;

    while (game.isRunning()) {
    game.handleEvents();

    if (game.getState() == MENU) {
        game.renderMenu();
    } else if (game.getState() == PLAYING) {
        game.update();
        game.render();
    }

    SDL_Delay(20);
    }


    return 0;
}
