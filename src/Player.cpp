#include "Player.h"
#include "game.h"
Player::Player(int x, int y) {
    rect = {x, y, 100, 100};
}

void Player::handleInput(SDL_Event& event, std::vector<Bullet>& bullets) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_LEFT) moveLeft = true;
        if (event.key.keysym.sym == SDLK_RIGHT) moveRight = true;
        if (event.key.keysym.sym == SDLK_UP) moveUp = true;
        if (event.key.keysym.sym == SDLK_DOWN) moveDown = true;
        if (event.key.keysym.sym == SDLK_LSHIFT) speedup = true;
        if (event.key.keysym.sym == SDLK_SPACE) {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastShotTime >= shootDelay) {
                bullets.push_back(Bullet(rect.x, rect.y));
                lastShotTime = currentTime;
            }
        }

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
    int moveSpeed = speedup ? speed + 20 : speed;

    if (moveLeft && rect.x > 0) rect.x -= moveSpeed;
    if (moveRight && rect.x < 1920 - rect.w) rect.x += moveSpeed;
    if (moveUp && rect.y > 0) rect.y -= moveSpeed;
    if (moveDown && rect.y < 1080 - rect.h) rect.y += moveSpeed;
    if(rect.x<0)rect.x = 0;
    if(rect.y<120)rect.y = 120;
    if(rect.x + 100 > 1920)rect.x = 1820;
    if(rect.y + 100 > 1080)rect.y = 980;


}
void Player::loadTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load Image Player: " << IMG_GetError() << std::endl;
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
void Player::setShootDelayByLevel(int level) {
    if (level <= 1) shootDelay = 400;
    else if (level == 2) shootDelay = 350;
    else if (level == 3) shootDelay = 250;
    else if (level == 4) shootDelay = 150;
    else if (level == 5) shootDelay = 50;
    else shootDelay = 0; // level 6 trở lên
}

void Player::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}
