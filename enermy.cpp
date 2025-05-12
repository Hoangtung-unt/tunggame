#include "enermy.h"
#include <iostream>
using namespace std;

Enemy::Enemy(SDL_Renderer* renderer, int x, Map* map) : x(x), alive(true) {
    SDL_Surface* surface = IMG_Load("animation/enemy.png");
    if (!surface) {
        cout << "Không thể tải ảnh enemy.png: " << IMG_GetError() << endl;
        return;
    }

    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 78, 79, 74));
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        cout << "Không thể tạo Texture từ Surface: " << SDL_GetError() << endl;
        return;
    }

    frame = 0;
    frameCount = 6;
    frameDelay = 8;
    frameDelayCounter = 0;

    y = 0;
    for (int ty = 0; ty < MAP_ROWS; ++ty) {
        int tile = map->GetTile(x / TILE_SIZE, ty);
        if (tile == 1 || tile == 2) {
            y = ty * TILE_SIZE - FRAME_HEIGHT;
            break;
        }
    }
}

void Enemy::Update(int playerX, int playerY, Map* map) {
    frameDelayCounter++;
    if (frameDelayCounter >= frameDelay) {
        frameDelayCounter = 0;
        frame = (frame + 1) % frameCount;
    }

    int nextX = x;
    if (x < playerX) {
        nextX += 2;
    } else if (x > playerX) {
        nextX -= 2;
    }

    int tileBelow = map->GetTile(nextX / TILE_SIZE, (y + FRAME_HEIGHT) / TILE_SIZE);
    if (tileBelow == 1 || tileBelow == 2) {
        x = nextX;
    }

    int groundY = (y + FRAME_HEIGHT) / TILE_SIZE * TILE_SIZE - FRAME_HEIGHT;
    if (map->GetTile(x / TILE_SIZE, groundY / TILE_SIZE) == 1 ||
        map->GetTile(x / TILE_SIZE, groundY / TILE_SIZE) == 2) {
        y = groundY;
    }

    if (x < 0) x = 0;
    if (x > SCREEN_WIDTH - FRAME_WIDTH) x = SCREEN_WIDTH - FRAME_WIDTH;
}

void Enemy::Render(SDL_Renderer* renderer, SDL_Rect camera) {
    if (!alive) return;

    SDL_Rect src = { frame * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT };
    SDL_Rect dst = { x - camera.x, y - camera.y, FRAME_WIDTH, FRAME_HEIGHT };

    SDL_RenderCopy(renderer, texture, &src, &dst);
}
