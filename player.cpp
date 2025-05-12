#include "player.h"
#include "map.h"
#include <iostream>
using namespace std;

Player::Player(SDL_Renderer* renderer,camera& cam) {
    SDL_Surface* surfaceRight = IMG_Load("animation/player_right.png");
    SDL_SetColorKey(surfaceRight, SDL_TRUE, SDL_MapRGB(surfaceRight->format, 167, 175, 180));
    textureRight = SDL_CreateTextureFromSurface(renderer, surfaceRight);
    SDL_FreeSurface(surfaceRight);
    SDL_Surface* surfaceLeft = IMG_Load("animation/player_left.png");
    SDL_SetColorKey(surfaceLeft, SDL_TRUE, SDL_MapRGB(surfaceLeft->format, 167, 175, 180));
    textureLeft = SDL_CreateTextureFromSurface(renderer, surfaceLeft);
    SDL_FreeSurface(surfaceLeft);
    SDL_Surface* surfaceJumpRight = IMG_Load("animation/jum_right.png");
    SDL_SetColorKey(surfaceJumpRight, SDL_TRUE, SDL_MapRGB(surfaceJumpRight->format, 167, 175, 180));
    textureJumpRight = SDL_CreateTextureFromSurface(renderer, surfaceJumpRight);
    SDL_FreeSurface(surfaceJumpRight);
    SDL_Surface* surfaceJumpLeft = IMG_Load("animation/jum_left.png");
    SDL_SetColorKey(surfaceJumpLeft, SDL_TRUE, SDL_MapRGB(surfaceJumpLeft->format, 167, 175, 180));
    textureJumpLeft = SDL_CreateTextureFromSurface(renderer, surfaceJumpLeft);
    SDL_FreeSurface(surfaceJumpLeft);
    currentTexture = textureRight;

    x = 0;
    y = -100;
    speed = PLAYER_SPEED;

    frame = 0;
    frameCount = 8;
    frameDelay = 8;
    frameDelayCounter = 0;
    moving = false;
    facingLeft = false;
    jumping = false;

    velocityY = 0;
    onGround = false;
    jumpPower = 20;

    lastShotTime = 0;
    shotCooldown = 300;
}
void Player::HandleInput(const Uint8* keyState, camera& cam) {
    moving = false;

    if (keyState[SDL_SCANCODE_RIGHT]) {
        x += speed;
        facingLeft = false;
        moving = true;

        if (jumping) {
            currentTexture = textureJumpRight;
        }
    }

    if (keyState[SDL_SCANCODE_LEFT]) {
        x -= speed;
        facingLeft = true;
        moving = true;

        if (jumping) {
            currentTexture = textureJumpLeft;
        }
    }

    if (keyState[SDL_SCANCODE_UP] && onGround) {
        jumping = true;
        velocityY = -jumpPower;
        onGround = false;
        currentTexture = facingLeft ? textureJumpLeft : textureJumpRight;
    }

    if (!jumping) {
        currentTexture = facingLeft ? textureLeft : textureRight;
    }

    Uint32 currentTime = SDL_GetTicks();

    if (keyState[SDL_SCANCODE_SPACE] && currentTime - lastShotTime >= shotCooldown) {
        int offsetX = facingLeft ? -10 : PLAYER_WIDTH + 10;
        int bulletX = x + offsetX;
        int bulletY = y;
        int bulletSpeed = 15;
        int vx = facingLeft ? -bulletSpeed : bulletSpeed;

        bullets.push_back(new Bullet(bulletX, bulletY, vx, 0));
        lastShotTime = currentTime;
    }
}



void Player::UpdateAnimation() {
    if (moving && !jumping) {
        frameDelayCounter++;
        if (frameDelayCounter >= frameDelay) {
            frameDelayCounter = 0;
            frame = (frame + 1) % frameCount;
        }
    } else {
        frame = 0;
    }
}

void Player::Update(Map* map, std::vector<Enemy*>& enemies) {
    UpdateAnimation();

    const int gravity = 1;
    const int maxFallSpeed = 10;

    if (!onGround) {
        velocityY += gravity;
        if (velocityY > maxFallSpeed)
            velocityY = maxFallSpeed;
    }

    y += velocityY;

    int tileX = (x + PLAYER_WIDTH / 2) / TILE_SIZE;
    int tileY = (y + PLAYER_HEIGHT) / TILE_SIZE;

    int id = map->GetTile(tileX, tileY);
    if (id == 1 || id == 2) {
        y = tileY * TILE_SIZE - PLAYER_HEIGHT;
        velocityY = 0;
        onGround = true;
        jumping = false;

        currentTexture = facingLeft ? textureLeft : textureRight;
    } else {
        onGround = false;
    }

    for (int i = 0; i < bullets.size(); ++i) {
        bullets[i]->Update(enemies);
        if (!bullets[i]->IsActive()) {
            delete bullets[i];
            bullets.erase(bullets.begin() + i);
            --i;
        }
    }

    if (x < 0) x = 0;
    if (x + PLAYER_WIDTH > MAP_COLS * TILE_SIZE)
        x = MAP_COLS * TILE_SIZE - PLAYER_WIDTH;
}


void Player::Render(SDL_Renderer* renderer, SDL_Rect camera) {
    SDL_Rect src = {
        frame * PLAYER_WIDTH,
        0,
        PLAYER_WIDTH,
        PLAYER_HEIGHT
    };

    SDL_Rect dst = {
        x - camera.x,
        y - camera.y,
        PLAYER_WIDTH,
        PLAYER_HEIGHT
    };

    SDL_RenderCopy(renderer, currentTexture, &src, &dst);

    for (Bullet* b : bullets) {
        b->Render(renderer, camera);
    }
}


Player::~Player() {
    SDL_DestroyTexture(textureRight);
    SDL_DestroyTexture(textureLeft);
    SDL_DestroyTexture(textureJumpRight);
    SDL_DestroyTexture(textureJumpLeft);
    for (Bullet* b : bullets) delete b;
    bullets.clear();
}
