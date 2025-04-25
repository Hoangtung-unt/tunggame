#include "player.h"
#include "map.h"
#include <iostream>
using namespace std;

Player::Player(SDL_Renderer* renderer) {
    // Tải sprite di chuyển phải
    SDL_Surface* surfaceRight = IMG_Load("animation/player_right.png");
    SDL_SetColorKey(surfaceRight, SDL_TRUE, SDL_MapRGB(surfaceRight->format, 167, 175, 180));
    textureRight = SDL_CreateTextureFromSurface(renderer, surfaceRight);
    SDL_FreeSurface(surfaceRight);

    // Tải sprite di chuyển trái
    SDL_Surface* surfaceLeft = IMG_Load("animation/player_left.png");
    SDL_SetColorKey(surfaceLeft, SDL_TRUE, SDL_MapRGB(surfaceLeft->format, 167, 175, 180));
    textureLeft = SDL_CreateTextureFromSurface(renderer, surfaceLeft);
    SDL_FreeSurface(surfaceLeft);

    // Tải sprite nhảy phải
    SDL_Surface* surfaceJumpRight = IMG_Load("animation/jum_right.png");
    SDL_SetColorKey(surfaceJumpRight, SDL_TRUE, SDL_MapRGB(surfaceJumpRight->format, 167, 175, 180));
    textureJumpRight = SDL_CreateTextureFromSurface(renderer, surfaceJumpRight);
    SDL_FreeSurface(surfaceJumpRight);

    // Tải sprite nhảy trái
    SDL_Surface* surfaceJumpLeft = IMG_Load("animation/jum_left.png");
    SDL_SetColorKey(surfaceJumpLeft, SDL_TRUE, SDL_MapRGB(surfaceJumpLeft->format, 167, 175, 180));
    textureJumpLeft = SDL_CreateTextureFromSurface(renderer, surfaceJumpLeft);
    SDL_FreeSurface(surfaceJumpLeft);

    // Khởi tạo giá trị nhân vật
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
    jumpPower = 15;

    lastShotTime = 0;
    shotCooldown = 300; // 300ms giữa mỗi lần bắn
}

void Player::HandleInput(const Uint8* keyState) {
    moving = false;

    if (keyState[SDL_SCANCODE_RIGHT]) {
        x += speed;
        facingLeft = false;
        moving = true;


        if (jumping) {  // Nếu đang nhảy, cập nhật sprite nhảy đúng hướng
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

    // Nếu nhân vật nhảy, chọn sprite nhảy đúng hướng
    if (keyState[SDL_SCANCODE_UP] && onGround) {
        jumping = true;
        velocityY = -jumpPower;
        onGround = false;

        currentTexture = facingLeft ? textureJumpLeft : textureJumpRight;
    }

    // Nếu không nhảy, trở về sprite di chuyển
    if (!jumping) {
        currentTexture = facingLeft ? textureLeft : textureRight;
    }
    Uint32 currentTime = SDL_GetTicks();

    if (keyState[SDL_SCANCODE_SPACE] && currentTime - lastShotTime >= shotCooldown) {
        int offsetX = facingLeft ? -10 : PLAYER_WIDTH + 10;  // nòng súng nhô ra ngoài nhân vật
        int offsetY = PLAYER_HEIGHT / 2 - 5;                 // tầm giữa thân người, chỉnh cho đẹp

        int bulletX = x + offsetX;
        int bulletY = 427;
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

void Player::Update(Map* map) {
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

        // Khi nhân vật tiếp đất, chuyển về sprite chạy
        currentTexture = facingLeft ? textureLeft : textureRight;
    } else {
        onGround = false;
    }

    // ===== Xử lý bắn đạn =====


    if (x < 0) x = 0;
    if (x + PLAYER_WIDTH > MAP_COLS * TILE_SIZE)
        x = MAP_COLS * TILE_SIZE - PLAYER_WIDTH;
    for (int i = 0; i < bullets.size(); ++i) {
        bullets[i]->Update();
        if (!bullets[i]->IsActive()) {
            delete bullets[i];
            bullets.erase(bullets.begin() + i);
            --i;
        }
    }
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
        b->Render(renderer);
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
