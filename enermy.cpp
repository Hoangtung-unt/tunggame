#include "enermy.h"
#include "map.h"
#include <iostream>
using namespace std;
Enemy::Enemy(SDL_Renderer* renderer, int x, int y) : x(x), y(y), alive(true) {
    // Tải sprite sheet
    SDL_Surface* surface = IMG_Load("animation/enemy.png");
    if (!surface) {
        cout << "Không thể tải ảnh enemy.png: " << IMG_GetError() << endl;
        return;
    }

    // Thiết lập Color Key để loại bỏ nền
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 78, 79, 74));

    // Chuyển Surface thành Texture
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        cout << "Không thể tạo Texture từ Surface: " << SDL_GetError() << endl;
        return;
    }

    // Khởi tạo các thông số animation
    frame = 0;
    frameCount = 6;       // Tổng số frame
    frameDelay = 8;       // Thời gian giữa các frame
    frameDelayCounter = 0;
    alive = true;
}

void Enemy::Update(int playerX, int playerY, Map* map) {
    // Cập nhật frame animation
    frameDelayCounter++;
    if (frameDelayCounter >= frameDelay) {
        frameDelayCounter = 0;
        frame = (frame + 1) % frameCount; // Thay đổi frame
    }

    // Tính toán hướng di chuyển ngang
    int nextX = x;
    if (x < playerX) {
        nextX += 2; // Di chuyển sang phải
    } else if (x > playerX) {
        nextX -= 2; // Di chuyển sang trái
    }

    // Kiểm tra tile đất dưới vị trí mới
    int tileBelow = map->GetTile(nextX / TILE_SIZE, (y + FRAME_HEIGHT) / TILE_SIZE);
    if (tileBelow == 1 || tileBelow == 2) { // Nếu tile là đất
        x = nextX; // Cập nhật vị trí ngang
    }

    // Cố định vị trí dọc để luôn đứng trên nền đất
    int groundY = (y + FRAME_HEIGHT) / TILE_SIZE * TILE_SIZE - FRAME_HEIGHT;
    if (map->GetTile(x / TILE_SIZE, groundY / TILE_SIZE) == 1 ||
        map->GetTile(x / TILE_SIZE, groundY / TILE_SIZE) == 2) {
        y = groundY;
    }
}

void Enemy::Render(SDL_Renderer* renderer, SDL_Rect camera) {
    if (!alive) return;

    SDL_Rect src = {
        frame * FRAME_WIDTH,
        0,
        FRAME_WIDTH,
        FRAME_HEIGHT
    };

    SDL_Rect dst = {
        x - camera.x,
        y - camera.y,
        FRAME_WIDTH,
        FRAME_HEIGHT
    };

    SDL_RenderCopy(renderer, texture, &src, &dst);
}

