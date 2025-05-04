#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class EnemyBullet {
public:
    EnemyBullet(int x, int y);
    void update();
    void render(SDL_Renderer* renderer);
    void loadTexture(SDL_Renderer* renderer, const std::string& path);
    bool isActive() const { return active; }
    void deactivate() { active = false; }
    SDL_Rect getRect() const { return rect; }

private:
    SDL_Rect rect;
    SDL_Texture* texture = nullptr;
    bool active = true;
};

#endif
