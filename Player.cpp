#include "Player.h"

Player::Player(int x, int y) {
    rect = {x, y, 50, 50};
}

void Player::handleInput(SDL_Event& event, std::vector<Bullet>& bullets) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_LEFT) moveLeft = true;
        if (event.key.keysym.sym == SDLK_RIGHT) moveRight = true;
        if (event.key.keysym.sym == SDLK_SPACE) bullets.push_back(Bullet(rect.x, rect.y));
    }
    else if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_LEFT) moveLeft = false;
        if (event.key.keysym.sym == SDLK_RIGHT) moveRight = false;
    }
}

void Player::update() {
    if (moveLeft && rect.x > 0) rect.x -= speed;
    if (moveRight && rect.x < 800 - rect.w) rect.x += speed;
}

void Player::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
