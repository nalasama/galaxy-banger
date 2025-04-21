#include "Bullet.h"

Bullet::Bullet(int x, int y) {
    rect = {x + 48 , y, 4, 40}; // Đạn bắn từ giữa nhân vật
}

void Bullet::update() {
    rect.y -= 20;
    if (rect.y < 0) active = false; // Nếu đạn ra khỏi màn hình thì biến mất
}
void Bullet::loadTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Lỗi tải ảnh Bullet: " << IMG_GetError() << std::endl;
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Bullet::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}
