#include "Game.h"
#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>

Game::Game() : window(nullptr), renderer(nullptr), running(false), backgroundTexture(nullptr) {}

Game::~Game() {
    clean();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if (TTF_Init() == -1) {
    std::cerr << "Lỗi khởi tạo SDL_ttf: " << TTF_GetError() << std::endl;
    return false;
}

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
    font = TTF_OpenFont("Times New Roman.ttf", 24);

    if (!font) {
        std::cerr << "Không thể tải font: " << TTF_GetError() << std::endl;
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

    // **Cập nhật tốc độ enemy dựa vào điểm số**
    enemySpeed = 2.0f + (score / 50);  // Cứ mỗi 50 điểm, enemy nhanh hơn

    // **Giảm thời gian spawn enemy khi đạt mốc điểm**
    if (score >= 100) enemySpawnRate = 40;
    if (score >= 200) enemySpawnRate = 30;
    if (score >= 300) enemySpawnRate = 20;
    if (score >= 500) enemySpawnRate = 10; // Khi đạt 500 điểm, enemy xuất hiện liên tục

    for (auto& bullet : bullets) bullet.update();
    for (auto& enemy : enemies) {
        enemy.update(enemySpeed);
    }

    for (auto& bullet : bullets) {
        for (auto& enemy : enemies) {
            if (checkCollision(bullet.getRect(), enemy.getRect())) {
                bullet.deactivate();
                enemy.deactivate();
                score += 10; // Cộng điểm khi bắn trúng enemy
            }
        }
    }

    for (auto& enemy : enemies) {
        enemy.update(enemySpeed);
        if (checkCollision(player->getRect(), enemy.getRect())) {
            running = false;
            std::cout << "Game Over! Player va cham voi Enemy.\n";
            return;
        }
        if (enemy.getRect().y > 600) { // Giả sử chiều cao màn hình là 600
        running = false;
        std::cout << "Game Over! Enemy ra khoi man hinh .\n";
        return;
    }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy& e) { return !e.isActive(); }), enemies.end());
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return !b.isActive(); }), bullets.end());

    // **Tạo enemy dựa trên enemySpawnRate**
    if (rand() % enemySpawnRate == 0) {
        enemies.push_back(Enemy(rand() % 750, -50));
    }
}




void Game::render() {
    SDL_RenderClear(renderer);

    // Vẽ nền
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    player->render(renderer);
    for (auto& bullet : bullets) bullet.render(renderer);
    for (auto& enemy : enemies) enemy.render(renderer);
    renderScore();
    SDL_RenderPresent(renderer);
}
void Game::renderScore() {
    SDL_Color textColor = {255, 255, 255, 255}; // Màu trắng
    std::string scoreText = "Score: " + std::to_string(score);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (!textSurface) return;

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_FreeSurface(textSurface);
        return;
    }

    // **Khởi tạo scoreRect trước khi sử dụng**
    scoreRect.x = 10;
    scoreRect.y = 10;
    scoreRect.w = textSurface->w;
    scoreRect.h = textSurface->h;

    SDL_FreeSurface(textSurface);

    SDL_RenderCopy(renderer, textTexture, NULL, &scoreRect);
    SDL_DestroyTexture(textTexture);
}



void Game::clean() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font); // **Đóng font**
    TTF_Quit(); // **Thoát SDL_ttf**
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
