//
// Created by taylo on 1/14/2026.
//

#include "Map.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>

#include "manager/TextureManager.h"
#include "vendor/tinyxml2.h"

void Map::loadLayerData(tinyxml2::XMLElement *layer, std::vector<std::vector<int> > &outData) {
    auto *data = layer->FirstChildElement("data");
    if (!data || !data->GetText()) {
        std::cout << "Layer missing CSV data" << std::endl;
        return;
    }

    std::string csv = data->GetText();
    std::stringstream ss(csv);

    outData = std::vector(height, std::vector<int>(width, 0));

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            std::string val;
            if (!std::getline(ss, val, ',')) {
                return;
            }
            outData[row][col] = std::stoi(val);
        }
    }
}

const Map::TilesetInfo *Map::getTilesetForGid(int gid) const {
    const TilesetInfo *best = nullptr;

    for (const auto &ts: tilesets) {
        if (gid >= ts.firstGid) {
            if (!best || ts.firstGid > best->firstGid) {
                best = &ts;
            }
        }
    }

    return best;
}

void Map::load(const char *path, SDL_Texture *backgroundTs, SDL_Texture *terrainTs) {
    if (path == nullptr) {
        std::cout << "no map to load" << std::endl;
        return;
    }

    colliders.clear();
    coins.clear();
    backgroundTileData.clear();
    terrainTileData.clear();
    tilesets.clear();

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(path) != tinyxml2::XML_SUCCESS) {
        std::cout << "failed to load map file: " << path << std::endl;
        return;
    }

    auto *mapNode = doc.FirstChildElement("map");
    if (!mapNode) {
        std::cout << "map node missing" << std::endl;
        return;
    }

    width = mapNode->IntAttribute("width");
    height = mapNode->IntAttribute("height");
    tileWidth = mapNode->IntAttribute("tilewidth");
    tileHeight = mapNode->IntAttribute("tileheight");

    // Parse tilesets from TMX
    for (auto *ts = mapNode->FirstChildElement("tileset");
         ts != nullptr;
         ts = ts->NextSiblingElement("tileset")) {
        TilesetInfo info;
        info.firstGid = ts->IntAttribute("firstgid");

        const char *sourceC = ts->Attribute("source");
        info.source = sourceC ? sourceC : "";

        // Match TMX source to the texture passed in
        if (info.source == "grassland.tsx") {
            info.texture = terrainTs;
        } else if (info.source == "grassland_bg.tsx") {
            info.texture = backgroundTs;
        } else {
            // ignore unknown tilesets
            continue;
        }

        float texW = 0.0f;
        float texH = 0.0f;
        SDL_GetTextureSize(info.texture, &texW, &texH);

        info.columns = static_cast<int>(texW) / tileWidth;
        int rows = static_cast<int>(texH) / tileHeight;
        info.tileCount = info.columns * rows;

        tilesets.push_back(info);
    }

    std::sort(tilesets.begin(), tilesets.end(),
              [](const TilesetInfo &a, const TilesetInfo &b) {
                  return a.firstGid < b.firstGid;
              });

    // Read layers by name
    for (auto *layer = mapNode->FirstChildElement("layer");
         layer != nullptr;
         layer = layer->NextSiblingElement("layer")) {
        const char *nameC = layer->Attribute("name");
        std::string layerName = nameC ? nameC : "";

        if (layerName == "Background Layer") {
            loadLayerData(layer, backgroundTileData);
        } else if (layerName == "Terrain Layer") {
            loadLayerData(layer, terrainTileData);
        }
    }

    // Read object groups
    for (auto *group = mapNode->FirstChildElement("objectgroup");
         group != nullptr;
         group = group->NextSiblingElement("objectgroup")) {
        const char *nameC = group->Attribute("name");
        std::string groupName = nameC ? nameC : "";

        if (groupName == "Collision Layer") {
            for (auto *obj = group->FirstChildElement("object");
                 obj != nullptr;
                 obj = obj->NextSiblingElement("object")) {
                Collider c;
                c.rect.x = obj->FloatAttribute("x");
                c.rect.y = obj->FloatAttribute("y");
                c.rect.w = obj->FloatAttribute("width");
                c.rect.h = obj->FloatAttribute("height");
                colliders.push_back(c);
            }
        } else if (groupName == "Item Layer") {
            for (auto *obj = group->FirstChildElement("object");
                 obj != nullptr;
                 obj = obj->NextSiblingElement("object")) {
                Collider c;
                c.rect.x = obj->FloatAttribute("x");
                c.rect.y = obj->FloatAttribute("y");
                coins.push_back(c);
            }
        }
    }
}

void Map::drawLayer(const Camera &cam, const std::vector<std::vector<int> > &layerData) {
    if (layerData.empty()) return;

    SDL_FRect src{}, dest{};
    dest.w = static_cast<float>(tileWidth) * cam.zoom;
    dest.h = static_cast<float>(tileHeight) * cam.zoom;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int gid = layerData[row][col];
            if (gid == 0) continue;

            const TilesetInfo *ts = getTilesetForGid(gid);
            if (!ts || !ts->texture) continue;

            int localId = gid - ts->firstGid;
            if (localId < 0 || localId >= ts->tileCount) continue;

            int srcCol = localId % ts->columns;
            int srcRow = localId / ts->columns;

            src.x = static_cast<float>(srcCol * tileWidth);
            src.y = static_cast<float>(srcRow * tileHeight);
            src.w = static_cast<float>(tileWidth);
            src.h = static_cast<float>(tileHeight);

            float worldX = static_cast<float>(col * tileWidth);
            float worldY = static_cast<float>(row * tileHeight);

            dest.x = std::round((worldX - cam.view.x) * cam.zoom);
            dest.y = std::round((worldY - cam.view.y) * cam.zoom);

            TextureManager::draw(ts->texture, &src, &dest);
        }
    }
}

void Map::draw(const Camera &cam) {
    drawLayer(cam, backgroundTileData);
    drawLayer(cam, terrainTileData);
}
