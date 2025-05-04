#include "EnemyBullet.h"

EnemyBullet::EnemyBullet(int x, int y) {
    rect = {x + 48, y + 100, 4, 20}; // Đạn bắn từ giữa enemy
}

void EnemyBullet::update() {
    rect.y += 3; // Bay xuống
    if (rect.y > 1080) active = false;
}



void EnemyBullet::render(SDL_Renderer* renderer) {
    if (texture)
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    else {
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255); // Màu đỏ nhạt
        SDL_RenderFillRect(renderer, &rect);
    }
}
