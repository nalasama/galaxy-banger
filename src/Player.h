#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "Bullet.h"

class Player {
public:
    Player(int x, int y);
    void handleInput(SDL_Event& event, std::vector<Bullet>& bullets);
    void loadTexture(SDL_Renderer* renderer, const std::string& path);
    void update();
    void render(SDL_Renderer* renderer);
    void setShootDelayByLevel(int level);

    SDL_Rect getRect() { return rect; }

private:
    Uint32 lastShotTime = 0; // thời điểm bắn gần nhất
    Uint32 shootDelay = 0; // thời gian delay giữa các lần bắn (ms) — ví dụ 100ms
    SDL_Rect rect;
    SDL_Texture* texture = nullptr;
    int speed = 10;
    bool moveLeft = false, moveRight = false;
    bool moveUp = false, moveDown = false;
    bool speedup = false;

};

#endif
