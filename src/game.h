#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <algorithm>
#include <string>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
//#include "render.h"
enum GameState { MENU, PLAYING, EXIT };

class Game {
public:
    Game();
    ~Game();
    bool init();
    void handleEvents();
    void update();
    void render();
    void clean();
    void renderMenu();
    GameState getState() { return gameState; }

    bool isRunning() { return running; }

private:
    GameState gameState = MENU;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Mix_Music* backgroundMusic;
    bool running;
    Player* player;
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    SDL_Texture* loadTexture(const std::string& path);
    SDL_Texture* backgroundTexture;
    bool checkCollision(SDL_Rect a, SDL_Rect b);
    float enemySpeed = 1.0f;
    int enemySpawnRate = 50;
    int level = 1;
    SDL_Rect levelRect;
    int score = 0;
    SDL_Rect scoreRect;

    void showGameOverScreen();
    void resetGame();
    TTF_Font* font;
    void renderScoreAndLevel();
    void renderHeaderBar();
};

#endif
