// //
// // Created by taylo on 1/14/2026.
// //
//
// #ifndef INC_8051TUTORIAL_MAP_H
// #define INC_8051TUTORIAL_MAP_H
//
// #include <vector>
// #include<SDL3/SDL.h>
// #include "ecs/Component.h"
//
// class Map {
// public:
//     Map() = default;
//
//     ~Map() = default;
//
//     void load(const char *path, SDL_Texture *ts);
//
//     void draw(const Camera &cam);
//
//     SDL_Texture *tileset = nullptr;
//     int width{}, height{};
//     std::vector<std::vector<int> > tileData;
//     std::vector<Collider> colliders;
//     std::vector<Collider> coins;
//     int getTileSize() const { return tileSize; }
// private:
//     //brace initialization
//     SDL_FRect src{}, dst{};
//     SDL_Texture *water = nullptr;
//     SDL_Texture *dirt = nullptr;
//     SDL_Texture *grass = nullptr;
//     int tileSize = 32;
// };
//
// #endif //INC_8051TUTORIAL_MAP_H

//
// Created by taylo on 1/14/2026.
//

//
// Created by taylo on 1/14/2026.
//

#ifndef INC_8051TUTORIAL_MAP_H
#define INC_8051TUTORIAL_MAP_H

#include <vector>
#include <string>
#include <SDL3/SDL.h>

#include "ecs/Component.h"
#include "vendor/tinyxml2.h"

class Map {
public:
    Map() = default;

    ~Map() = default;

    void load(const char *path, SDL_Texture *backgroundTs, SDL_Texture *terrainTs);

    void draw(const Camera &cam);

    int getTileSize() const { return tileWidth; }

    std::vector<Collider> colliders;
    std::vector<Collider> coins;
    int width = 0;
    int height = 0;
    int tileWidth = 16;
    int tileHeight = 16;

private:
    struct TilesetInfo {
        int firstGid = 0;
        int tileCount = 0;
        int columns = 0;
        SDL_Texture *texture = nullptr;
        std::string source;
    };


    std::vector<std::vector<int> > backgroundTileData;
    std::vector<std::vector<int> > terrainTileData;

    std::vector<TilesetInfo> tilesets;

    void loadLayerData(tinyxml2::XMLElement *layer, std::vector<std::vector<int> > &outData);

    void drawLayer(const Camera &cam, const std::vector<std::vector<int> > &layerData);

    const TilesetInfo *getTilesetForGid(int gid) const;
};

#endif //INC_8051TUTORIAL_MAP_H
