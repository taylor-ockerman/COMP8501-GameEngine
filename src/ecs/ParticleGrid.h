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


struct Cell {
    ParticleType type = ParticleType::Empty;
    //this entity bridges ECS and cellular automata grid
    Entity* entity = nullptr;
};

class ParticleGrid {
public:
    ParticleGrid(int worldWidth, int worldHeight, int cellSize) : cellSize(cellSize) {
        width = worldWidth / cellSize;
        height = worldHeight / cellSize;
        cells.resize(height * width);
    };

    int getCellSize() const {return cellSize;};
    int getHeight() const {return height;};
    int getWidth() const {return width;};
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
        Cell& cell = at(gx, gy);
        cell.type = ParticleType::Empty;
        cell.entity->destroy();
        cell.entity = nullptr;
    }

    void clearGrid() {
        for (auto& cell : cells) {
            cell.type = ParticleType::Empty;
            cell.entity = nullptr;
        }
    }
    void update() {
        for (int y = height - 1; y >= 0; y--) {
            for (int x = 0; x < width; x++) {
                {
                    switch (at(x, y).type) {
                        case ParticleType::Sand:
                            updateSand(x, y);
                            //std::cout << "updating sand"<< std::endl;
                            break;
                        case ParticleType::Water:
                            updateWater(x, y);
                            std::cout << "updating water"<< std::endl;
                            break;
                        case ParticleType::Stone:
                            updateStone(x, y);
                            //std::cout << "updating stone"<< std::endl;
                            break;
                        case ParticleType::Empty:
                        default:
                            break;
                    }
                }
            }
        }
    }

    void updateSand(int x, int y) {
        //check directly below
        if (isEmpty(x, y + 1)) {
            swapCells(x, y, x, y + 1);
            return;
        }

        //randomize checking left or right first to add some variance to particle movements
        bool leftFirst = rand() % 2 == 0;

        if (leftFirst) {
            if (isEmpty(x - 1, y + 1)) {
                swapCells(x - 1, y + 1, x, y);
                return;
            }
            if (isEmpty(x + 1, y + 1)) {
                swapCells(x + 1, y + 1, x, y);
                return;
            }
        } else {
            if (isEmpty(x + 1, y + 1)) {
                swapCells(x + 1, y + 1, x, y);
                return;
            }
            if (isEmpty(x - 1, y + 1)) {
                swapCells(x - 1, y + 1, x, y);
                return;
            }
        }
    }

    void updateWater(int x, int y) {
        //check directly below
        if (isEmpty(x, y + 1)) {
            swapCells(x, y, x, y + 1);
            return;
        }

        //randomize checking left or right first to add some variance to particle movements
        bool leftFirst = rand() % 2 == 0;

        if (leftFirst) {
            if (isEmpty(x - 1, y + 1)) {
                swapCells(x - 1, y + 1, x, y);
                return;
            }
            if (isEmpty(x + 1, y + 1)) {
                swapCells(x + 1, y + 1, x, y);
                return;
            }
        } else {
            if (isEmpty(x + 1, y + 1)) {
                swapCells(x + 1, y + 1, x, y);
                return;
            }
            if (isEmpty(x - 1, y + 1)) {
                swapCells(x - 1, y + 1, x, y);
                return;
            }
        }
        //if diagonals dont work, move sideways
        if (leftFirst) {
            if (isEmpty(x - 1, y)) {
                swapCells(x - 1, y, x, y);
                return;
            }
            if (isEmpty(x + 1, y)) {
                swapCells(x + 1, y, x, y);
                return;
            }
        } else {
            if (isEmpty(x + 1, y)) {
                swapCells(x + 1, y, x, y);
                return;
            }
            if (isEmpty(x - 1, y)) {
                swapCells(x - 1, y, x, y);
                return;
            }
        }
    }

    void updateStone(int x, int y) {
        //shouldnt move so nothing
    }

    bool spawnParticleAtCell(int gx, int gy, ParticleType type, Entity& entity) {
        //int gx = x / cellSize;
        //int gy = y / cellSize;
        if (!inBounds(gx, gy) || !isEmpty(gx, gy)) return false;
        Cell& cell = at(gx, gy);
        cell.type = type;
        cell.entity = &entity;
        std::cout << "spawnParticleAtCell: gx=" << gx
          << " gy=" << gy
          << " type=" << static_cast<int>(type)
          << std::endl;
        return true;
    };

private:
    int width;
    int height;
    int cellSize;
    std::vector<Cell> cells;
};

#endif //INC_8051TUTORIAL_PARTICLEGRIDSYSTEM_H
