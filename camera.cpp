#include "camera.h"

camera::camera(int screenWidth, int screenHeight, int mapWidth, int mapHeight)
    : screenW(screenWidth), screenH(screenHeight),
      mapW(mapWidth), mapH(mapHeight)
{
    view = { 0, 0, screenW, screenH };
}

void camera::Follow(int targetX, int targetY) {
    view.x = targetX - screenW / 2;
    view.y = targetY - screenH / 2;

    if (view.x < 0) view.x = 0;
    if (view.y < 0) view.y = 0;
    if (view.x > mapW - screenW) view.x = mapW - screenW;
    if (view.y > mapH - screenH) view.y = mapH - screenH;
}

SDL_Rect camera::GetView() const {
    return view;
}


