#ifndef ENEMY_H
#define ENEMY_H
#include <iostream>
#include <SDL_image.h>
#include <SDL.h>
#include <string>

class Enemy {
public:
    Enemy(int x, int y);
    void update(float speed, SDL_Rect playerRect);
    void loadTexture(SDL_Renderer* renderer, const std::string& path);
    void render(SDL_Renderer* renderer);
    bool isActive() { return active; }
    void deactivate() { active = false; }
    SDL_Rect getRect() { return rect; }
    Uint32 getLastShotTime() const { return lastShotTime; }
    void setLastShotTime(Uint32 time) { lastShotTime = time; }


private:
    SDL_Texture* texture = nullptr;
    SDL_Rect rect;
    bool active = true;
    Uint32 lastShotTime = 2000;

};

#endif
