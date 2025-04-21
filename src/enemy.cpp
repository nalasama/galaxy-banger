#include "Enemy.h"

Enemy::Enemy(int x, int y) {
    rect = {x, y, 100, 100};
}

void Enemy::update(float speed) {
    rect.y += speed; // **Dùng tốc độ động thay vì cố định 2**
    if (rect.y > 1080) active = false; // Enemy biến mất khi ra khỏi màn hình
}
void Enemy::loadTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Lỗi tải ảnh Enemy: " << IMG_GetError() << std::endl;
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Enemy::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}
