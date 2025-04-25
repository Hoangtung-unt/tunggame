#include "bullet.h"
#include "constants.h"
#include "enermy.h"  // Bao gồm Enemy để sử dụng lớp Enemy trong kiểm tra va chạm
#include <iostream>
Bullet::Bullet(int x, int y, int speedX, int speedY)
    : x(x), y(y), speedX(speedX), speedY(speedY), active(true) {}

void Bullet::Update(std::vector<Enemy*>& enemies) {
    if (!active) return;

    // Cập nhật vị trí viên đạn
    x += speedX;
    y += speedY;

    // Kiểm tra va chạm với các quái vật
    for (Enemy* enemy : enemies) {
        if (active && enemy->IsAlive() && CheckCollision(enemy)) {
            // Nếu có va chạm, hủy hoạt động của viên đạn và hạ quái vật
            Deactivate();
            enemy->SetAlive(false);  // Hạ quái vật
            break;
        }
    }

    // Kiểm tra nếu viên đạn ra ngoài màn hình
    if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) {
        active = false;  // Tắt viên đạn nếu nó ra ngoài màn hình
    }
}

bool Bullet::CheckCollision(Enemy* enemy) {
    SDL_Rect bulletRect = GetRect();         // Lấy hình chữ nhật của viên đạn
    SDL_Rect enemyRect = enemy->GetRect();   // Lấy hình chữ nhật của quái vật

    // Kiểm tra va chạm giữa viên đạn và quái vật
    return SDL_HasIntersection(&bulletRect, &enemyRect);
}

void Bullet::Render(SDL_Renderer* renderer) {
    if (!active) return;

    SDL_Rect rect = { x, y, WIDTH, HEIGHT };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Màu đỏ cho đạn
    SDL_RenderFillRect(renderer, &rect);
}

SDL_Rect Bullet::GetRect() const {
    return { x, y, WIDTH, HEIGHT };
}

void Bullet::Deactivate() {
    active = false;
}
