#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>

class Bullet {
public:
    Bullet(int x, int y, int speedX, int speedY);
    void Update();
    void Render(SDL_Renderer* renderer);
    bool IsActive() const { return active; }
    void SetActive(bool isActive) { active = isActive; }

    int GetX() const { return x; }
    int GetY() const { return y; }

private:
    int x, y;
    int speedX, speedY;
    bool active;
    int WIDTH = 10;  // Kích thước viên đạn
    int HEIGHT = 10; // Kích thước viên đạn
};

#endif
