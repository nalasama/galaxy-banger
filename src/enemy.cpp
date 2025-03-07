#include "Enemy.h"

Enemy::Enemy(int x, int y) {
    rect = {x, y, 50, 50};
}

void Enemy::update() {
    rect.y += 3;
    if (rect.y > 600) active = false; // Enemy biến mất khi ra khỏi màn hình
}

void Enemy::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
