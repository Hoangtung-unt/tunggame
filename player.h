#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "constants.h"
#include "bullet.h"
#include <vector>
#include "camera.h"

class Map;

class Player {
public:
    Player(SDL_Renderer* renderer, camera& camera);
    ~Player();

    void HandleInput(const Uint8* keyState, camera& cam);
    void Update(Map* map, std::vector<Enemy*>& enemies);

    void Render(SDL_Renderer* renderer, SDL_Rect camera);

    int GetX() const { return x; }
    int GetY() const { return y; }

    std::vector<Bullet*>& GetBullets() { return bullets; }

private:
    SDL_Texture* textureRight;
    SDL_Texture* textureLeft;
    SDL_Texture* textureJumpRight;
    SDL_Texture* textureJumpLeft;
    SDL_Texture* currentTexture;

    int x, y;
    int speed;

    int frame;
    int frameCount;
    int frameDelay;
    int frameDelayCounter;
    bool moving;
    bool facingLeft;

    bool jumping;
    int jumpPower;

    int velocityY;
    bool onGround;

    void UpdateAnimation();

    std::vector<Bullet*> bullets;
    Uint32 lastShotTime;
    Uint32 shotCooldown;

};

#endif
