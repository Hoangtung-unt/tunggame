#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "constants.h"

class Map;

class Player {
public:
    Player(SDL_Renderer* renderer);
    ~Player();

    void HandleInput(const Uint8* keyState);
    void Update(Map* map);
    void Render(SDL_Renderer* renderer, SDL_Rect camera);

    int GetX() const { return x; }
    int GetY() const { return y; }

private:
    SDL_Texture* textureRight;
    SDL_Texture* textureLeft;
    SDL_Texture* textureJumpRight; // Thêm biến sprite nhảy phải
    SDL_Texture* textureJumpLeft;  // Thêm biến sprite nhảy trái
    SDL_Texture* currentTexture;

    int x, y;
    int speed;

    // Animation
    int frame;
    int frameCount;
    int frameDelay;
    int frameDelayCounter;
    bool moving;
    bool facingLeft;

    // Nhảy
    bool jumping;   // Kiểm tra trạng thái nhảy
    int jumpPower;  // Lực nhảy

    // Trọng lực & vận tốc Y
    int velocityY;
    bool onGround;

    void UpdateAnimation();
};

#endif
