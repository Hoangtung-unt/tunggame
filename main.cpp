#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <ctime>
#include <iostream> // Thêm để sử dụng std::cout
#include "map.h"
#include "camera.h"
#include "player.h"
#include "enermy.h" // 👈 Đã có
#include "constants.h"

// Hàm kiểm tra va chạm
bool CheckCollision(const SDL_Rect& rect1, const SDL_Rect& rect2) {
    return !(rect1.x + rect1.w <= rect2.x || // Không chạm bên trái
             rect1.x >= rect2.x + rect2.w || // Không chạm bên phải
             rect1.y + rect1.h <= rect2.y || // Không chạm bên trên
             rect1.y >= rect2.y + rect2.h);  // Không chạm bên dưới
}

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

        // Cập nhật các quái vật
        for (Enemy* enemy : enemies) {
            enemy->Update(player.GetX(), player.GetY(), &map);
        }

        // Kiểm tra va chạm giữa nhân vật và quái vật
        SDL_Rect playerRect = { player.GetX(), player.GetY(), PLAYER_WIDTH, PLAYER_HEIGHT };
        for (Enemy* enemy : enemies) {
            SDL_Rect enemyRect = { enemy->GetX(), enemy->GetY(), FRAME_WIDTH, FRAME_HEIGHT };
            if (CheckCollision(playerRect, enemyRect)) {
                std::cout << "Game Over: Quái vật đã chạm vào nhân vật!" << std::endl;
                running = false; // Kết thúc trò chơi
                break;
            }
        }

        // Cập nhật camera theo nhân vật
        camera.Follow(player.GetX(), player.GetY());

        // Vẽ màn hình
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Xóa màn hình với màu xanh trời
        SDL_RenderClear(renderer);

        map.Render(camera.GetView());

        for (Enemy* enemy : enemies) {
            enemy->Render(renderer, camera.GetView());
        }

        player.Render(renderer, camera.GetView());

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    // Hiển thị thông báo Game Over trước khi kết thúc
    SDL_Delay(2000); // Tạm dừng để hiển thị thông báo Game Over

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
