#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>
#include <vector>

class Enemy;  // Khai báo trước để tránh lỗi circular include

class Bullet {
public:
    Bullet(int x, int y, int speedX, int speedY);

    // 🛠 Sửa: Khai báo đúng hàm Update mới
    void Update(std::vector<Enemy*>& enemies);

    void Render(SDL_Renderer* renderer);
    bool IsActive() const { return active; }
    void SetActive(bool isActive) { active = isActive; }

    int GetX() const { return x; }
    int GetY() const { return y; }

    SDL_Rect GetRect() const;
    void Deactivate();

private:
    int x, y;
    int speedX, speedY;
    bool active;
    int WIDTH = 20;
    int HEIGHT = 20;

    // 🛠 Sửa: Thêm khai báo CheckCollision đúng
    bool CheckCollision(Enemy* enemy);
};

#endif
