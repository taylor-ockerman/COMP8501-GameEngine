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
    bool occupiedByPlayer = false;
};

struct Chunk {
    bool active = true;
    bool movedThisFrame = false;
    int idleFrames = 0;
};

class ParticleGrid {
public:
    ParticleGrid(int worldWidth, int worldHeight, int cellSize) : cellSize(cellSize) {
        width = worldWidth / cellSize;
        height = worldHeight / cellSize;
        cells.resize(height * width);

        chunkWidth = (width + chunkSize - 1) / chunkSize;
        chunkHeight = (height + chunkSize - 1) / chunkSize;
        chunks.resize(chunkWidth * chunkHeight);

        for (int i = 0; i < static_cast<int>(ParticleType::Last); i++) {
            particleTypeCountMap.emplace(static_cast<ParticleType>(i), 0);
        }
    };
    //world coordinates to grid coordinates
    int worldToGridX(float worldX) const {
        return static_cast<int>(worldX) / cellSize;
    }

    int worldToGridY(float worldY) const {
        return static_cast<int>(worldY) / cellSize;
    }

    //grid coordinates to world coordinates
    int gridToWorldX(int gx) const {
        return gx * cellSize;
    }

    int gridToWorldY(int gy) const {
        return gy * cellSize;
    }

    int getCellSize() const { return cellSize; };
    int getHeight() const { return height; };
    int getWidth() const { return width; };
    std::vector<Chunk> &getChunks() { return chunks; };
    int getChunkSize() const { return chunkSize; };
    int getChunkWidth() const { return chunkWidth; };
    int getChunkHeight() const { return chunkHeight; };

    bool inBounds(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    };

    bool inBoundsChunk(int cx, int cy) const {
        return cx >= 0 && cx < chunkWidth && cy >= 0 && cy < chunkHeight;
    }

    Chunk &chunkAt(int cx, int cy) {
        return chunks[cy * chunkWidth + cx];
    }

    int getChunkX(int gx) {
        return gx / chunkSize;
    }

    int getChunkY(int gy) {
        return gy / chunkSize;
    }

    Chunk &getChunkFromCell(int gx, int gy) {
        return chunkAt(getChunkX(gx), getChunkY(gy));
    }

    void wakeChunk(int cx, int cy) {
        if (!inBoundsChunk(cx, cy)) return;

        Chunk &chunk = chunkAt(cx, cy);
        chunk.active = true;
        chunk.idleFrames = 0;
    }

    void wakeChunkForCell(int gx, int gy) {
        if (!inBounds(gx, gy)) return;
        wakeChunk(getChunkX(gx), getChunkY(gy));
    }

    void wakeChunkAndNeighborsForCell(int gx, int gy) {
        if (!inBounds(gx, gy)) return;
        int cx = getChunkX(gx);
        int cy = getChunkY(gy);
        for (int y = cy - 1; y <= cy + 1; y++) {
            for (int x = cx - 1; x <= cx + 1; x++) {
                wakeChunk(x, y);
            }
        }
    }

    Cell &at(int gx, int gy) {
        return cells[gy * width + gx];
    };

    bool isEmpty(int x, int y) const {
        return inBounds(x, y) && cells[y * width + x].type == ParticleType::Empty;
    };

    void swapCells(int x1, int y1, int x2, int y2) {
        std::swap(at(x1, y1), at(x2, y2));
        getChunkFromCell(x1, y1).movedThisFrame = true;
        getChunkFromCell(x2, y2).movedThisFrame = true;
        wakeChunkAndNeighborsForCell(x1, y1);
        wakeChunkAndNeighborsForCell(x2, y2);
    }

    void clearCell(int gx, int gy) {
        if (!inBounds(gx, gy)) return;
        Cell &cell = at(gx, gy);
        particleTypeCountMap.at(cell.type)--;
        if (cell.entity != nullptr) {
            cell.entity->destroy();
        }
        cell.type = ParticleType::Empty;
        cell.behaviour = ParticleBehaviour::Static;
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

    std::vector<std::pair<int, int> > &getPreviousPlayerCells() {
        return previousPlayerCells;
    }

    //helper functions used for debugging
    int getActiveChunkCount() {
        int count = 0;
        for (auto &chunk: chunks) {
            if (chunk.active) count++;
        }
        return count;
    }

    int countNonEmptyCells() {
        int count = 0;
        for (auto &cell: cells) {
            if (cell.type != ParticleType::Empty && cell.type != ParticleType::Wall) count++;
        }
        return count;
    }

    //returns coordinates of all cells immediately around the given coordinate
    //used for locality checks during particle interactions
    std::vector<std::pair<int, int> > getNeighbours(int gx, int gy) {
        std::vector<std::pair<int, int> > neighbours{};
        if (!inBounds(gx, gy)) return neighbours;
        for (int x = gx - 1; x <= gx + 1; x++) {
            for (int y = gy - 1; y <= gy + 1; y++) {
                if (inBounds(x, y)) neighbours.push_back(std::pair<int, int>(x, y));
            }
        }
        return neighbours;
    }

private:
    int width;
    int height;
    int cellSize;
    int chunkSize = 16;
    int chunkWidth;
    int chunkHeight;
    std::vector<Cell> cells;
    std::vector<Chunk> chunks;
    std::vector<std::pair<int, int> > previousPlayerCells;
    //helps to reduce area to search for an update player position from entire grid to limited parts of the grid
    std::unordered_map<ParticleType, int> particleTypeCountMap{};
};

#endif //INC_8051TUTORIAL_PARTICLEGRIDSYSTEM_H
