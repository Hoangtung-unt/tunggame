#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <ctime>
#include <iostream>
#include "map.h"
#include "camera.h"
#include "player.h"
#include "enermy.h"
#include "constants.h"

bool CheckCollision(const SDL_Rect& rect1, const SDL_Rect& rect2) {
    return !(rect1.x + rect1.w <= rect2.x ||
             rect1.x >= rect2.x + rect2.w ||
             rect1.y + rect1.h <= rect2.y ||
             rect1.y >= rect2.y + rect2.h);
}

void ShowStartScreen(SDL_Renderer* renderer) {
    TTF_Font* font = TTF_OpenFont("assets/my.font.ttf", 24);
    if (!font) {
        std::cerr << "Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color textColor = { 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Press ENTER to Start", textColor);
    if (!textSurface) {
        std::cerr << "Unable to create text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Rect textRect;
    textRect.x = (SCREEN_WIDTH - 300) / 2;
    textRect.y = (SCREEN_HEIGHT - 50) / 2;
    textRect.w = 300;
    textRect.h = 50;

    bool startGame = false;
    while (!startGame) {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_RenderPresent(renderer);

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) startGame = true;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) startGame = true;
        }
    }

    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

void ShowGameOverScreen(SDL_Renderer* renderer) {
    TTF_Font* font = TTF_OpenFont("assets/my.font.ttf", 48);
    if (!font) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color textColor = {255, 0, 0};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Game Over", textColor);
    if (!textSurface) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Rect textRect;
    textRect.w = 300;
    textRect.h = 80;
    textRect.x = (SCREEN_WIDTH - textRect.w) / 2;
    textRect.y = (SCREEN_HEIGHT - textRect.h) / 2;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_RenderPresent(renderer);

    SDL_Delay(2000);

    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

void ShowYouWinScreen(SDL_Renderer* renderer) {
    TTF_Font* font = TTF_OpenFont("assets/my.font.ttf", 48);
    if (!font) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color textColor = { 0, 255, 0 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "YOU WIN!", textColor);
    if (!textSurface) {
        std::cerr << "Unable to render win text! SDL_ttf Error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Rect textRect;
    textRect.w = 300;
    textRect.h = 80;
    textRect.x = (SCREEN_WIDTH - textRect.w) / 2;
    textRect.y = (SCREEN_HEIGHT - textRect.h) / 2;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_RenderPresent(renderer);

    SDL_Delay(2000);

    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Scrolling Map + Camera",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, MAP_COLS * TILE_SIZE, MAP_ROWS * TILE_SIZE);
    Map map(renderer);
    Player player(renderer, camera);

    srand(static_cast<unsigned int>(time(NULL)));
    std::vector<Enemy*> enemies;
    for (int i = 0; i < 5; ++i) {
        int randX = SCREEN_WIDTH + rand() % 300;
        enemies.push_back(new Enemy(renderer, randX, &map));
    }

    Uint32 lastSpawnTime = SDL_GetTicks();
    const Uint32 spawnInterval = 2000;

    ShowStartScreen(renderer);

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastSpawnTime + spawnInterval) {
            int randX = SCREEN_WIDTH + rand() % 300;
            enemies.push_back(new Enemy(renderer, randX, &map));
            lastSpawnTime = currentTime;
        }

        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        player.HandleInput(keyState, camera);
        player.Update(&map, enemies);

        for (Enemy* enemy : enemies) {
            enemy->Update(player.GetX(), player.GetY(), &map);
        }

        for (Enemy* enemy : enemies) {
            if (!enemy->IsAlive()) continue;

            for (Bullet* bullet : player.GetBullets()) {
                if (!bullet->IsActive()) continue;

                if (CheckCollision(bullet->GetRect(), enemy->GetRect())) {
                    enemy->SetAlive(false);
                    bullet->Deactivate();
                    std::cout << "💥 Enemy trúng đạn tại: (" << enemy->GetX() << ", " << enemy->GetY() << ")\n";
                    break;
                }
            }
        }

        for (auto it = enemies.begin(); it != enemies.end(); ) {
            if (!(*it)->IsAlive()) {
                delete *it;
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }

        auto& bullets = player.GetBullets();
        for (auto it = bullets.begin(); it != bullets.end(); ) {
            if (!(*it)->IsActive()) {
                delete *it;
                it = bullets.erase(it);
            } else {
                ++it;
            }
        }

        SDL_Rect playerRect = { player.GetX(), player.GetY(), PLAYER_WIDTH, PLAYER_HEIGHT };
        for (Enemy* enemy : enemies) {
            if (CheckCollision(playerRect, enemy->GetRect())) {
                std::cout << "Game Over: Quái vật đã chạm vào nhân vật!" << std::endl;

                ShowGameOverScreen(renderer);

                running = false;
                break;
            }
        }
        int frontTileX = (player.GetX() + PLAYER_WIDTH) / TILE_SIZE;
        int midTileY = (player.GetY() + PLAYER_HEIGHT / 2) / TILE_SIZE;
        if (map.GetTile(frontTileX, midTileY) == 5) {
            std::cout << "YOU WIN: Nhân vật đã đến đích!\n";
            ShowYouWinScreen(renderer);
            running = false;
            continue;
        }

        camera.Follow(player.GetX(), player.GetY());

        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
        SDL_RenderClear(renderer);

        map.Render(camera.GetView());

        for (Enemy* enemy : enemies) {
            enemy->Render(renderer, camera.GetView());
        }

        player.Render(renderer, camera.GetView());

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    for (Enemy* enemy : enemies) {
        delete enemy;
    }

    for (Enemy* enemy : enemies) {
        delete enemy;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
