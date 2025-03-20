#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <algorithm>
#include <string>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "render.h"
class Game {
public:
    Game();
    ~Game();
    bool init();
    void handleEvents();
    void update();
    void render();
    void clean();
    bool isRunning() { return running; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    Player* player;
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    SDL_Texture* loadTexture(const std::string& path);
    SDL_Texture* backgroundTexture;
    bool checkCollision(SDL_Rect a, SDL_Rect b);
    //int score = 0;
    float enemySpeed = 2.0f;
    int enemySpawnRate = 50;
    SDL_Rect scoreRect; // Vị trí và kích thước của điểm số
    // **Thêm biến lưu điểm số**
    int score = 0;

    // **Thêm biến font để hiển thị điểm**
    TTF_Font* font;

    void renderScore();

};

#endif
