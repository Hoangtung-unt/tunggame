#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "constants.h"
#include "bullet.h"
#include <vector>
#include "camera.h"  // Thêm vào để sử dụng class camera

class Map;

class Player {
public:
    Player(SDL_Renderer* renderer, camera& camera);  // Thêm camera* vào constructor
    ~Player();

    void HandleInput(const Uint8* keyState, camera& cam);  // Thêm camera vào HandleInput
    void Update(Map* map, std::vector<Enemy*>& enemies);

    void Render(SDL_Renderer* renderer, SDL_Rect camera);

    int GetX() const { return x; }
    int GetY() const { return y; }

    const std::vector<Bullet*>& GetBullets() const { return bullets; }

private:
    SDL_Texture* textureRight;
    SDL_Texture* textureLeft;
    SDL_Texture* textureJumpRight;
    SDL_Texture* textureJumpLeft;
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
    bool jumping;
    int jumpPower;

    // Trọng lực & vận tốc Y
    int velocityY;
    bool onGround;

    void UpdateAnimation();

    std::vector<Bullet*> bullets;
    Uint32 lastShotTime;
    Uint32 shotCooldown;

};

#endif
