#ifndef BULLET_H
#define BULLET_H
#include <iostream>
#include <SDL_image.h>
#include <SDL.h>
#include <string>

class Bullet {
public:
    Bullet(int x, int y);
    void update();
    void loadTexture(SDL_Renderer* renderer, const std::string& path);
    void render(SDL_Renderer* renderer);
    bool isActive() { return active; }
    void deactivate() { active = false; }
    SDL_Rect getRect() { return rect; }

private:
    SDL_Rect rect;
    SDL_Texture* texture = nullptr;
    bool active = true;
};

#endif
