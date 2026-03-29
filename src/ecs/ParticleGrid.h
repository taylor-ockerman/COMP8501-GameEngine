//
// Created by taylo on 3/18/2026.
//

#ifndef INC_8051TUTORIAL_PARTICLEGRIDSYSTEM_H
#define INC_8051TUTORIAL_PARTICLEGRIDSYSTEM_H
#include <cstdlib>
#include <vector>
#include <SDL3/SDL_render.h>
#include <iostream>
#include "Component.h"
#include "Entity.h"
#include "ParticleHelpers.h"


struct Cell {
    ParticleType type = ParticleType::Empty;
    ParticleBehaviour behaviour = ParticleBehaviour::Static;
    //this entity bridges ECS and cellular automata grid
    Entity *entity = nullptr;
};

class ParticleGrid {
public:
    ParticleGrid(int worldWidth, int worldHeight, int cellSize) : cellSize(cellSize) {
        width = worldWidth / cellSize;
        height = worldHeight / cellSize;
        cells.resize(height * width);
    };

    int getCellSize() const { return cellSize; };
    int getHeight() const { return height; };
    int getWidth() const { return width; };

    bool inBounds(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    };

    Cell &at(int x, int y) {
        return cells[y * width + x];
    };

    bool isEmpty(int x, int y) const {
        return inBounds(x, y) && cells[y * width + x].type == ParticleType::Empty;
    };

    void swapCells(int x1, int y1, int x2, int y2) {
        std::swap(at(x1, y1), at(x2, y2));
    }

    void clearCell(int gx, int gy) {
        if (!inBounds(gx, gy)) return;
        Cell &cell = at(gx, gy);
        cell.type = ParticleType::Empty;
        if (cell.entity != nullptr) {
            cell.entity->destroy();
        }
        cell.entity = nullptr;
    }

    void clearGrid() {
        for (auto &cell: cells) {
            cell.type = ParticleType::Empty;
            cell.entity = nullptr;
        }
    }

    //fills cells in the grid based on wall collider rect geometry, so we can avoid tons of collision events between particles and walls.
    void addWallRect(const SDL_FRect &wallRect) {
        int startGX = static_cast<int>(wallRect.x) / cellSize;
        int endGX = static_cast<int>(wallRect.x + wallRect.w - 1) / cellSize;
        int startGY = static_cast<int>(wallRect.y) / cellSize;
        int endGY = static_cast<int>(wallRect.y + wallRect.h - 1) / cellSize;

        for (int gy = startGY; gy <= endGY; gy++) {
            for (int gx = startGX; gx <= endGX; gx++) {
                if (!inBounds(gx, gy)) continue;
                Cell &cell = at(gx, gy);
                cell.type = ParticleType::Wall;
                cell.behaviour = ParticleBehaviour::Static;
                cell.entity = nullptr;
            }
        }
    }

private
:
    int width;
    int height;
    int cellSize;
    std::vector<Cell> cells;
};

#endif //INC_8051TUTORIAL_PARTICLEGRIDSYSTEM_H
