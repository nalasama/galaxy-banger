#include "Game.h"
#include <iostream>
#include <SDL_image.h>

Game::Game() : window(nullptr), renderer(nullptr), running(false), backgroundTexture(nullptr) {}

Game::~Game() {
    clean();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Init Failed: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Space Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    // Load ảnh nền
    backgroundTexture = loadTexture("galaxy.jpg");
    if (!backgroundTexture) {
        std::cerr << "Lỗi: Không thể tải ảnh nền!" << std::endl;
        return false;
    }

    player = new Player(375, 500);
    running = true;
    return true;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
        player->handleInput(event, bullets);
    }
}

void Game::update() {
    player->update();

    for (auto& bullet : bullets) bullet.update();
    for (auto& enemy : enemies) enemy.update();

    for (auto& bullet : bullets) {
        for (auto& enemy : enemies) {
            if (checkCollision(bullet.getRect(), enemy.getRect())) {
                bullet.deactivate();
                enemy.deactivate();
            }
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy& e) { return !e.isActive(); }), enemies.end());
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return !b.isActive(); }), bullets.end());

    if (rand() % 50 == 0) enemies.push_back(Enemy(rand() % 750, -50));
}

void Game::render() {
    SDL_RenderClear(renderer);

    // Vẽ nền
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    player->render(renderer);
    for (auto& bullet : bullets) bullet.render(renderer);
    for (auto& enemy : enemies) enemy.render(renderer);

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Game::checkCollision(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y);
}

SDL_Texture* Game::loadTexture(const std::string& path) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        std::cerr << "Lỗi tải ảnh: " << IMG_GetError() << std::endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}
