#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <SDL_image.h>
#include "constants.h"
#include "map.h"

class Enemy {
public:
    Enemy(SDL_Renderer* renderer, int x, Map* map);

    void Update(int playerX, int playerY, Map* map);
    void Render(SDL_Renderer* renderer, SDL_Rect camera);

    bool IsAlive() const { return alive; }
    void SetAlive(bool a) { alive = a; }

    int GetX() const { return x; }
    int GetY() const { return y; }

    SDL_Rect GetRect() const {
        return { x, y, FRAME_WIDTH, FRAME_HEIGHT };
    }

private:
    SDL_Texture* texture;
    int x, y;
    int frame;
    int frameCount;
    int frameDelay;
    int frameDelayCounter;
    bool alive;
};

#endif
