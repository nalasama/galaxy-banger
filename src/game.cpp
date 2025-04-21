#include "Game.h"
#include "Player.h"
#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
using namespace std;

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

    window = SDL_CreateWindow("Space Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    // Load ảnh nền
    backgroundTexture = loadTexture("galaxy.jpg");
    if (!backgroundTexture) {
        std::cerr << "Lỗi: Không thể tải ảnh nền!" << std::endl;
        return false;
    }
    font = TTF_OpenFont("Times New Roman.ttf", 100);

    if (!font) {
        std::cerr << "Không thể tải font: " << TTF_GetError() << std::endl;
        return false;
    }
    player = new Player(960, 980);
    player->loadTexture(renderer, "player.png");

    for (auto& bullet : bullets) {
        bullet.loadTexture(renderer, "bullet.png");
    }
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

    SDL_Rect playRect = {840, 450, playSurface->w, playSurface->h};
    SDL_Rect quitRect = {840, 600, quitSurface->w, quitSurface->h};

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
    if (score >= 200){
        level = 2;

    }
    if (score >= 300){
        enemySpawnRate = 40;
        level = 3;
    }
    if (score >= 400){
        enemySpawnRate = 30;
        level = 4;
    }
    if (score >= 500) {
        enemySpawnRate = 20;
        level = 5;
    }
    if (score >= 600) {
        enemySpawnRate = 10;
        level = 6;
    }
    player->setShootDelayByLevel(level);




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
            if (checkCollision(player->getRect(), enemy.getRect())) {
                enemy.deactivate();
                score += 10; // Cộng điểm va cham voi enemy
            }
        }

    for (auto& enemy : enemies) {
        enemy.update(enemySpeed);
        //checkCollision(player->getRect(), enemy.getRect()) ||
        if(enemy.getRect().y > 1080) {
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
        Enemy newEnemy(rand() % 1820, -100);
        newEnemy.loadTexture(renderer, "enemy.png");
        enemies.push_back(newEnemy);
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
    SDL_Rect gameOverRect = {(1920 - surface->w) / 2, (1080 - surface->h) / 2, surface->w, surface->h};

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
    player = new Player(960, 980); // Đặt lại vị trí nhân vật
    player->loadTexture(renderer, "player.png");
    bullets.clear(); // Xóa tất cả đạn
    enemies.clear(); // Xóa tất cả enemy
    enemySpeed = 0.5f;
    enemySpawnRate = 60;
    level = 1;
    running = true; // Chạy lại game
}




void Game::render() {
    SDL_RenderClear(renderer);

    // Vẽ nền
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    player->render(renderer);
    for (auto& bullet : bullets) bullet.render(renderer);
    for (auto& enemy : enemies) enemy.render(renderer);
    renderScoreAndLevel();
    SDL_RenderPresent(renderer);
}
void Game::renderHeaderBar() {
    // Vẽ thanh nền đen ở đầu màn hình
    SDL_Rect headerRect;
    headerRect.x = 0;
    headerRect.y = 0;
    headerRect.w = 1920;
    headerRect.h = 120; // chiều cao thanh

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // màu đen
    SDL_RenderFillRect(renderer, &headerRect);
}

void Game::renderScoreAndLevel() {
    renderHeaderBar(); // Vẽ nền trước

    // ----- Score -----
    SDL_Color textColor = {255, 255, 255, 255}; // màu trắng
    std::string scoreText = "Score: " + std::to_string(score);

    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (!scoreSurface) return;

    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    if (!scoreTexture) {
        SDL_FreeSurface(scoreSurface);
        return;
    }

    SDL_Rect scoreRect;
    scoreRect.x = 10;
    scoreRect.y = 10;
    scoreRect.w = scoreSurface->w;
    scoreRect.h = scoreSurface->h;

    SDL_FreeSurface(scoreSurface);
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
    SDL_DestroyTexture(scoreTexture);

    // ----- Level -----
    std::string levelText = "Level: " + std::to_string(level);
    SDL_Surface* levelSurface = TTF_RenderText_Solid(font, levelText.c_str(), textColor);
    if (!levelSurface) return;

    SDL_Texture* levelTexture = SDL_CreateTextureFromSurface(renderer, levelSurface);
    if (!levelTexture) {
        SDL_FreeSurface(levelSurface);
        return;
    }

    SDL_Rect levelRect;
    levelRect.w = levelSurface->w;
    levelRect.h = levelSurface->h;
    levelRect.x = 1920 - levelRect.w - 10; // canh phải
    levelRect.y = 10;

    SDL_FreeSurface(levelSurface);
    SDL_RenderCopy(renderer, levelTexture, NULL, &levelRect);
    SDL_DestroyTexture(levelTexture);
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


bool Game::checkCollision(SDL_Rect a,SDL_Rect b) {
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
