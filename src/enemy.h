#ifndef ENEMY_H
#define ENEMY_H
#include <iostream>
#include <SDL_image.h>
#include <SDL.h>
#include <string>

class Enemy {
public:
    Enemy(int x, int y);
    void update(float speed);
    void loadTexture(SDL_Renderer* renderer, const std::string& path);
    void render(SDL_Renderer* renderer);
    bool isActive() { return active; }
    void deactivate() { active = false; }
    SDL_Rect getRect() { return rect; }

private:
    SDL_Texture* texture = nullptr;
    SDL_Rect rect;
    bool active = true;
};

#endif
