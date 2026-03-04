//
// Created by taylo on 1/14/2026.
//

#ifndef INC_8051TUTORIAL_MAP_H
#define INC_8051TUTORIAL_MAP_H

#include <vector>
#include<SDL3/SDL.h>
#include "ecs/Component.h"

class Map {
public:
    Map() = default;

    ~Map() = default;

    void load(const char *path, SDL_Texture *ts);

    void draw(const Camera &cam);

    SDL_Texture *tileset = nullptr;
    int width{}, height{};
    std::vector<std::vector<int> > tileData;
    std::vector<Collider> colliders;
    std::vector<Collider> coins;

private:
    //brace initialization
    SDL_FRect src{}, dst{};
    SDL_Texture *water = nullptr;
    SDL_Texture *dirt = nullptr;
    SDL_Texture *grass = nullptr;

    //int map[10][15]{};
};

#endif //INC_8051TUTORIAL_MAP_H
