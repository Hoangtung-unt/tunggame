#ifndef ENERMY_H
#define ENERMY_H

#include <SDL.h>
#include <SDL_image.h>
#include "constants.h"
#include "map.h"

class Enemy {
public:
    Enemy(SDL_Renderer* renderer, int x, int y);
    void Update(int playerX, int playerY, Map* map);
    void Render(SDL_Renderer* renderer, SDL_Rect camera);

    int GetX() const { return x; }
    int GetY() const { return y; }

    SDL_Rect GetRect() const {
        return { x, y, FRAME_WIDTH, FRAME_HEIGHT };
    }

    bool IsAlive() const { return alive; }
    void SetAlive(bool a) { alive = a; }

private:
    SDL_Texture* texture;
    int x, y;

    int frame;
    int frameCount;
    int frameDelay;
    int frameDelayCounter;
    int currentFrame;

    bool alive;
};

#endif
