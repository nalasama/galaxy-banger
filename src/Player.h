#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <vector>
#include "Bullet.h"

class Player {
public:
    Player(int x, int y);
    void handleInput(SDL_Event& event, std::vector<Bullet>& bullets);
    void update();
    void render(SDL_Renderer* renderer);
    SDL_Rect getRect() { return rect; }

private:
    SDL_Rect rect;
    int speed = 5;
    bool moveLeft = false, moveRight = false,speedup = false,speeddown = false;
};

#endif
