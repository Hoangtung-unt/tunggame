#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <vector>
#include <string>
#include <map>

const int TILE_SIZE = 32;
const int MAP_ROWS = 12;
const int MAP_COLS =400;

class Map {
public:
    Map(SDL_Renderer* renderer);
    ~Map();
    void LoadMap(const std::string& filename);
    void Render(SDL_Rect camera);
    int GetTile(int x, int y);

private:
    SDL_Renderer* renderer;
    std::map<int, SDL_Texture*> tileTextures;
    int tileMap[MAP_ROWS][MAP_COLS];

    void LoadTile(int id, const std::string& path);
};

#endif
