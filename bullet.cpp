#include "bullet.h"
#include "constants.h"
#include "enermy.h"
#include <iostream>
Bullet::Bullet(int x, int y, int speedX, int speedY)
    : x(x), y(y), speedX(speedX), speedY(speedY), active(true) {}

void Bullet::Update(std::vector<Enemy*>& enemies) {
    if (!active) return;
    x += speedX;
    y += speedY;
    for (Enemy* enemy : enemies) {
        if (active && enemy->IsAlive() && CheckCollision(enemy)) {
            Deactivate();
            enemy->SetAlive(false);
            break;
        }
    }

    if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) {
        active = false;
    }
}

bool Bullet::CheckCollision(Enemy* enemy) {
    SDL_Rect bulletRect = GetRect();
    SDL_Rect enemyRect = enemy->GetRect();
    return SDL_HasIntersection(&bulletRect, &enemyRect);
}

void Bullet::Render(SDL_Renderer* renderer, SDL_Rect camera) {
    if (!active) return;

    SDL_Rect rect = { x - camera.x, y - camera.y, WIDTH, HEIGHT };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}



SDL_Rect Bullet::GetRect() const {
    return { x, y, WIDTH, HEIGHT };
}

void Bullet::Deactivate() {
    active = false;
}
