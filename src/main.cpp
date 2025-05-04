#include "Game.h"

int SDL_main(int argc, char* argv[]) {
    Game game;
    if (!game.init()) return -1;

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    while (game.isRunning()) {
        frameStart = SDL_GetTicks();

        game.handleEvents();

        if (game.getState() == MENU) {
            game.renderMenu();
        } else if (game.getState() == PLAYING) {
            game.update();
            game.render();
        }

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return 0;
}
