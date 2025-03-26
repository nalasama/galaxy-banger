#include "Player.h"
Player::Player(int x, int y) {
    rect = {x, y, 50, 50};
}

void Player::handleInput(SDL_Event& event, std::vector<Bullet>& bullets) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_LEFT) moveLeft = true;
        if (event.key.keysym.sym == SDLK_RIGHT) moveRight = true;
        if (event.key.keysym.sym == SDLK_UP) moveUp = true;
        if (event.key.keysym.sym == SDLK_DOWN) moveDown = true;
        if (event.key.keysym.sym == SDLK_LSHIFT) speedup = true;
        if (event.key.keysym.sym == SDLK_SPACE) bullets.push_back(Bullet(rect.x, rect.y));
    }
    else if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_LEFT) moveLeft = false;
        if (event.key.keysym.sym == SDLK_RIGHT) moveRight = false;
        if (event.key.keysym.sym == SDLK_UP) moveUp = false;
        if (event.key.keysym.sym == SDLK_DOWN) moveDown = false;
        if (event.key.keysym.sym == SDLK_LSHIFT) speedup = false;
    }
}

void Player::update() {
      int moveSpeed = speedup ? speed + 10 : speed;
//  which mean:
//    int moveSpeed;
//    if (speedup) {
//        moveSpeed = speed + 10;
//    } else {
//        moveSpeed = speed;
//    }


    if (moveLeft && rect.x > 0) rect.x -= moveSpeed;
    if (moveRight && rect.x < 800 - rect.w) rect.x += moveSpeed;
    if (moveUp && rect.y > 0) rect.y -= moveSpeed;
    if (moveDown && rect.y < 600 - rect.h) rect.y += moveSpeed;
}
void Player::loadTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Lỗi tải ảnh Player: " << IMG_GetError() << std::endl;
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Player::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}
