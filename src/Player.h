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
    SDL_Rect getRect() { return rect; }

private:
    SDL_Rect rect;
    SDL_Texture* texture = nullptr;
    int speed = 5;
    bool moveLeft = false, moveRight = false;
    bool moveUp = false, moveDown = false;
    bool speedup = false;

};

#endif
