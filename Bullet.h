#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>

class Bullet {
public:
    Bullet(int x, int y);
    void update();
    void render(SDL_Renderer* renderer);
    bool isActive() { return active; }
    void deactivate() { active = false; }
    SDL_Rect getRect() { return rect; }

private:
    SDL_Rect rect;
    bool active = true;
};

#endif
