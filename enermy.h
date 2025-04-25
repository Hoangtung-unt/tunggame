#ifndef ENERMY_H
#define ENERMY_H

#include <SDL.h>
#include <SDL_image.h>
#include "constants.h"
#include <SDL.h>

class Enemy {
public:
    Enemy(SDL_Renderer* renderer, int x, int y);
    void Update();
    void Render(SDL_Renderer* renderer, SDL_Rect camera);

    int GetX() const { return x; }
    int GetY() const { return y; }

private:
    SDL_Texture* texture;
    int x, y;

    int frame;
    int frameCount;
    int frameDelay;
    int frameDelayCounter;
    int currentFrame = 0;
    const int FRAME_WIDTH = 74.95;
    const int FRAME_HEIGHT = 80;
};

#endif
