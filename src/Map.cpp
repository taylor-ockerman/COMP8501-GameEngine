//
// Created by taylo on 1/14/2026.
//

#include "Map.h"

#include <cmath>
#include <iostream>

#include "TextureManager.h"
#include <sstream>
#include "vendor/tinyxml2.h"

void Map::load(const char *path, SDL_Texture *ts) {
    tileset = ts;
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    //parse width and height of map
    auto *mapNode = doc.FirstChildElement("map");
    if (mapNode == nullptr) {
        std::cout << "no map to load" << std::endl;
        return;
    }
    width = mapNode->IntAttribute("width");
    height = mapNode->IntAttribute("height");
    //parse terrain data
    auto *layer = mapNode->FirstChildElement("layer");
    auto *data = layer->FirstChildElement("data");
    std::string csv = data->GetText();
    std::stringstream ss(csv);
    tileData = std::vector(height, std::vector<int>(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::string val;
            //read characters from a ss into val until it hits a comma, or it is at the end of the stream
            if (!std::getline(ss, val, ','))break;

            tileData[i][j] = std::stoi(val); //stoi is a string to int converter
        }
    }
    for (auto *group = mapNode->FirstChildElement("objectgroup");
         group != nullptr;
         group = group->NextSiblingElement("objectgroup")) {
        const char *nameC = group->Attribute("name");
        std::string groupName = nameC ? nameC : "";

        if (groupName == "Collider Layer") {
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
                // coins are points, so no width/height
                coins.push_back(c);
            }
        }
        // else: ignore other groups (or handle more later)
    }
    //next sibling element
    // auto *objectGroup = layer->NextSiblingElement("objectgroup");
    // //create a for loop with initialization and an increment
    // for (auto *obj = objectGroup->FirstChildElement("object");
    //      obj != nullptr;
    //      obj = obj->NextSiblingElement("object")) {
    //     Collider c;
    //     c.rect.x = obj->FloatAttribute("x");
    //     c.rect.y = obj->FloatAttribute("y");
    //     c.rect.w = obj->FloatAttribute("width");
    //     c.rect.h = obj->FloatAttribute("height");
    //     colliders.push_back(c);
    // }
    // auto *coinGroup = objectGroup->NextSiblingElement("objectgroup");
    // for (auto *obj = coinGroup->FirstChildElement("object");
    //      obj != nullptr;
    //      obj = obj->NextSiblingElement("object")) {
    //     Collider c;
    //     c.rect.x = obj->FloatAttribute("x");
    //     c.rect.y = obj->FloatAttribute("y");
    //     //shouldn't have width of height since coins are placed using points, not boxes
    //     //c.rect.w = obj->FloatAttribute("width");
    //     //c.rect.h = obj->FloatAttribute("height");
    //     coins.push_back(c);
    // }
}

void Map::draw(const Camera &cam) {
    SDL_FRect src{}, dest{};
    dest.w = dest.h = 32;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int type = tileData[row][col];


            float worldX = static_cast<float>(col) * dest.w;
            float worldY = static_cast<float>(row) * dest.h;

            //move the tiles or map relative to the camera
            //convert from world space to screen space
            dest.x = std::round(worldX - cam.view.x);
            dest.y = std::round(worldY - cam.view.y);

            switch (type) {
                case 1: //dirt
                    src.x = 0;
                    src.y = 0;
                    src.w = 32;
                    src.h = 32;
                    break;
                case 2: //grass
                    src.x = 32;
                    src.y = 0;
                    src.w = 32;
                    src.h = 32;
                    break;
                case 4: //water
                    src.x = 32;
                    src.y = 32;
                    src.w = 32;
                    src.h = 32;
                    break;
                default:
                    break;
            }
            //std::cout << src.x << " " << src.y << " " << src.w << " " << src.h << std::endl;
            TextureManager::draw(tileset, src, dest);
        }
    }
}
