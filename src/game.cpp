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
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cerr << "Lỗi khởi tạo SDL_mixer: " << Mix_GetError() << std::endl;
    return false;
    }

    backgroundMusic = Mix_LoadMUS("Elden Ring theme.mp3");
    if (!backgroundMusic) {
        std::cerr << "Không thể tải nhạc nền: " << Mix_GetError() << std::endl;
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
    player->loadTexture(renderer, "player.png");
    for (auto& enemy : enemies) {
        enemy.loadTexture(renderer, "enemy.png");
    }
    for (auto& bullet : bullets) {
        bullet.loadTexture(renderer, "bullet.png");
    }
    player = new Player(375, 500);
    running = true;
    return true;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gameState = EXIT;
            running = false; // Dừng game
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_1: // Chọn "Play" từ menu
                    if (gameState == MENU) {
                        gameState = PLAYING;
                        if (Mix_PausedMusic()) {
                            Mix_ResumeMusic(); // Tiếp tục phát nhạc từ chỗ dừng
                        } else {
                            Mix_PlayMusic(backgroundMusic, -1); // Phát lại nếu chưa phát trước đó
                        }
                    }
                    break;
                case SDLK_2:
                    if (gameState == MENU) {
                        gameState = EXIT;
                        running = false;
                    }
                    break;
                case SDLK_ESCAPE:
                     if (gameState == PLAYING) {
                        gameState = MENU;
                        Mix_PauseMusic(); // Tạm dừng nhạc thay vì dừng hẳn
                    }
                    break;
            }

            if (gameState == PLAYING) {
                player->handleInput(event, bullets);
            }
        }
        if (event.type == SDL_KEYUP && gameState == PLAYING) {
            player->handleInput(event, bullets);
        }
    }
}


void Game::renderMenu() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = {255, 255, 255, 255};

    SDL_Surface* playSurface = TTF_RenderText_Solid(font, "1. Play", textColor);
    SDL_Surface* quitSurface = TTF_RenderText_Solid(font, "2. Quit", textColor);

    SDL_Texture* playTexture = SDL_CreateTextureFromSurface(renderer, playSurface);
    SDL_Texture* quitTexture = SDL_CreateTextureFromSurface(renderer, quitSurface);

    SDL_Rect playRect = {350, 200, playSurface->w, playSurface->h};
    SDL_Rect quitRect = {350, 300, quitSurface->w, quitSurface->h};

    SDL_RenderCopy(renderer, playTexture, NULL, &playRect);
    SDL_RenderCopy(renderer, quitTexture, NULL, &quitRect);

    SDL_RenderPresent(renderer);

    SDL_FreeSurface(playSurface);
    SDL_FreeSurface(quitSurface);
    SDL_DestroyTexture(playTexture);
    SDL_DestroyTexture(quitTexture);
}



void Game::update() {
    player->update();

    // **Cập nhật tốc độ enemy dựa vào điểm số**
    enemySpeed = 1.0f + (score / 100);  // Cứ mỗi 100 điểm, enemy nhanh hơn

    // **Giảm thời gian spawn enemy khi đạt mốc điểm**
    if (score >= 300) enemySpawnRate = 40;
    if (score >= 400) enemySpawnRate = 30;
    if (score >= 500) enemySpawnRate = 20;
    if (score >= 600) enemySpawnRate = 10; // Khi đạt 600 điểm, enemy xuất hiện liên tục

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
        if (checkCollision(player->getRect(), enemy.getRect()) || enemy.getRect().y > 600) {
            showGameOverScreen();
            return;
        }

    if (!running) {
    gameState = MENU;
    running = true;
    }

    }



    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy& e) { return !e.isActive(); }), enemies.end());
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return !b.isActive(); }), bullets.end());

    // **Tạo enemy dựa trên enemySpawnRate**
    if (rand() % enemySpawnRate == 0) {
        enemies.push_back(Enemy(rand() % 750, -50));
    }
}

void Game::showGameOverScreen() {
    Mix_HaltMusic(); // Dừng hẳn nhạc khi game over

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = {255, 0, 0, 255}; // Màu đỏ
    SDL_Surface* surface = TTF_RenderText_Solid(font, "GAME OVER", textColor);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect gameOverRect = {(800 - surface->w) / 2, (600 - surface->h) / 2, surface->w, surface->h};

    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &gameOverRect);
    SDL_DestroyTexture(texture);
    SDL_RenderPresent(renderer);

    SDL_Delay(1000); // Hiển thị màn hình Game Over trong 3 giây

    gameState = MENU;
    resetGame(); // Reset game để chơi lại
}

void Game::resetGame() {
    score = 0; // Đặt lại điểm số
    player = new Player(375, 500); // Đặt lại vị trí nhân vật
    bullets.clear(); // Xóa tất cả đạn
    enemies.clear(); // Xóa tất cả enemy
    enemySpeed = 1.0f;
    enemySpawnRate = 50;
    running = true; // Chạy lại game
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
    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();
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
