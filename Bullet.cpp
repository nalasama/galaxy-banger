#include "Bullet.h"

Bullet::Bullet(int x, int y) {
    rect = {x + 20, y, 10, 20}; // Đạn bắn từ giữa nhân vật
}

void Bullet::update() {
    rect.y -= 10;
    if (rect.y < 0) active = false; // Nếu đạn ra khỏi màn hình thì biến mất
}

void Bullet::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
