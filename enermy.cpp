#include "enermy.h"
#include "map.h"
Enemy::Enemy(SDL_Renderer* renderer, int x, int y) : x(x), y(y) {
    SDL_Surface* surface = IMG_Load("animation/enemy.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    frame = 0;
    frameCount = 6;       // ví dụ 4 frame
    frameDelay = 8;
    frameDelayCounter = 0;
}

void Enemy::Update() {
    // Cập nhật animation frame
    frameDelayCounter++;
    if (frameDelayCounter >= frameDelay) {
        frameDelayCounter = 0;
        frame = (frame + 1) % frameCount;
    }
}

void Enemy::Render(SDL_Renderer* renderer, SDL_Rect camera) {
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
