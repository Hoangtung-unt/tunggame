#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <ctime>
#include "map.h"
#include "camera.h"
#include "player.h"
#include "enermy.h" // 👈 Thêm dòng này
#include "constants.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Scrolling Map + Camera",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load map
    Map map(renderer);

    // Tạo player và camera
    Player player(renderer);
    camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, MAP_COLS * TILE_SIZE, MAP_ROWS * TILE_SIZE);

    // Khởi tạo random seed và sinh enemy
    srand(static_cast<unsigned int>(time(NULL)));
    std::vector<Enemy*> enemies;

    for (int i = 0; i < 10; ++i) {
        int randX = rand() % (MAP_COLS * TILE_SIZE);
        int randY = 0;

        // Tìm tile đất đầu tiên từ trên xuống tại cột randX
        for (int y = 0; y < MAP_ROWS; ++y) {
            int tile = map.GetTile(randX / TILE_SIZE, y);
            if (tile == 1 || tile == 2) {
                randY = y * TILE_SIZE - 80; // Đặt enemy ngay trên tile đó
                break;
            }
        }

        enemies.push_back(new Enemy(renderer, randX, randY));
    }

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        // Xử lý phím nhấn
        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        player.HandleInput(keyState);
        player.Update(&map);

        for (Enemy* enemy : enemies) {
    enemy->Update(player.GetX(), player.GetY(), &map);
}

        // Cập nhật camera theo nhân vật
        camera.Follow(player.GetX(), player.GetY());

        // Vẽ màn hình
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
        SDL_RenderClear(renderer);

        map.Render(camera.GetView());

        for (Enemy* enemy : enemies) {
            enemy->Render(renderer, camera.GetView());
        }

        player.Render(renderer, camera.GetView());

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60fps
    }

    // Dọn bộ nhớ
    for (Enemy* enemy : enemies) {
        delete enemy;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
