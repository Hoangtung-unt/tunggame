#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>

class camera {
public:
    camera(int screenWidth, int screenHeight, int mapWidth, int mapHeight);

    void Follow(int targetX, int targetY);
    SDL_Rect GetView() const;

private:
    SDL_Rect view;
    int screenW, screenH;
    int mapW, mapH;
};

#endif

