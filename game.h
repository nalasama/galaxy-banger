#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include <algorithm>
#include <string>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

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
};

#endif
