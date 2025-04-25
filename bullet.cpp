#include "bullet.h"
#include "constants.h"
Bullet::Bullet(int x, int y, int speedX, int speedY)
    : x(x), y(y), speedX(speedX), speedY(speedY), active(true) {}

void Bullet::Update() {
    if (!active) return;

    x += speedX;
    y += speedY;

    if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) {
        active = false;
    }
}

void Bullet::Render(SDL_Renderer* renderer) {
    if (!active) return;

    SDL_Rect rect = { x, y, WIDTH, HEIGHT };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Màu đỏ cho đạn
    SDL_RenderFillRect(renderer, &rect);
}
