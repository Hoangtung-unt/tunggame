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

// Hàm kiểm tra va chạm
bool CheckCollision(const SDL_Rect& rect1, const SDL_Rect& rect2) {
    return !(rect1.x + rect1.w <= rect2.x ||
             rect1.x >= rect2.x + rect2.w ||
             rect1.y + rect1.h <= rect2.y ||
             rect1.y >= rect2.y + rect2.h);
}

void ShowStartScreen(SDL_Renderer* renderer) {
    TTF_Font* font = TTF_OpenFont("assets/fonts/my.font", 24);
if (font == nullptr) {
    std::cerr << "Error: " << TTF_GetError() << std::endl;
}

    SDL_Color textColor = { 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Press ENTER to Start", textColor);
    if (textSurface == NULL) {
        std::cerr << "Unable to create text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        std::cerr << "Unable to create texture from text surface! SDL Error: " << SDL_GetError() << std::endl;
    }
    SDL_FreeSurface(textSurface);

    SDL_Rect textRect;
    textRect.x = (SCREEN_WIDTH - textSurface->w) / 2;
    textRect.y = (SCREEN_HEIGHT - textSurface->h) / 2;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    bool startGame = false;
    while (!startGame) {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_RenderPresent(renderer);

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                startGame = true;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
                startGame = true;
            }
        }
    }

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

    for (int i = 0; i < 10; ++i) {
        int randX = rand() % (MAP_COLS * TILE_SIZE);
        int randY = 0;

        for (int y = 0; y < MAP_ROWS; ++y) {
            int tile = map.GetTile(randX / TILE_SIZE, y);
            if (tile == 1 || tile == 2) {
                randY = y * TILE_SIZE - 80;
                break;
            }
        }

        enemies.push_back(new Enemy(renderer, randX, randY));
    }

    ShowStartScreen(renderer);

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                {running = false;}
        }

        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        player.HandleInput(keyState, camera);
        player.Update(&map, enemies);
        for (Enemy* enemy : enemies) {
            enemy->Update(player.GetX(), player.GetY(), &map);
        }

        SDL_Rect playerRect = { player.GetX(), player.GetY(), PLAYER_WIDTH, PLAYER_HEIGHT };
        for (Enemy* enemy : enemies) {
            SDL_Rect enemyRect = { enemy->GetX(), enemy->GetY(), FRAME_WIDTH, FRAME_HEIGHT };
            if (CheckCollision(playerRect, enemyRect)) {
                std::cout << "Game Over: Quái vật đã chạm vào nhân vật!" << std::endl;
                running = false;
                break;
            }
        }

        for (Enemy* enemy : enemies) {
            if (!enemy->IsAlive()) continue;

            for (Bullet* bullet : player.GetBullets()) {
                if (!bullet->IsActive()) continue;

                SDL_Rect enemyRect = enemy->GetRect();
                if (CheckCollision(bullet->GetRect(), enemyRect)) {
                    enemy->SetAlive(false);
                    bullet->Deactivate();
                    std::cout << "💥 Enemy trúng đạn tại: (" << enemy->GetX() << ", " << enemy->GetY() << ")\n";
                    break;
                }
            }
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

    SDL_Delay(2000);

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
