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
        //if (!canDisplace(x1, y1, x2, y2)) return;
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

    void update() {
        for (int y = height - 1; y >= 0; y--) {
            for (int x = 0; x < width; x++) {
                {
                    ParticleBehaviour bh = at(x, y).behaviour;
                    if (bh == ParticleBehaviour::Gas) continue;
                    switch (bh) {
                        case ParticleBehaviour::Powder:
                            updatePowder(x, y);
                            //std::cout << "updating sand" << std::endl;
                            break;
                        case ParticleBehaviour::Liquid:
                            updateLiquid(x, y);
                            //std::cout << "updating water"<< std::endl;
                            break;
                        case ParticleBehaviour::Static:
                            updateStatic(x, y);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (at(x, y).behaviour != ParticleBehaviour::Gas
                ) {
                    continue;
                } else {
                    updateGas(x, y);
                }
            }
        }
    }

    void updatePowder(int x, int y) {
        //check directly below
        if (canDisplace(x, y, x, y + 1)) {
            swapCells(x, y, x, y + 1);
            return;
        }

        //randomize checking left or right first to add some variance to powder movements
        bool leftFirst = rand() % 2 == 0;

        if (leftFirst) {
            if (canDisplace(x, y, x - 1, y + 1)) {
                swapCells(x - 1, y + 1, x, y);
                return;
            }
            if (canDisplace(x, y, x + 1, y + 1)) {
                swapCells(x + 1, y + 1, x, y);
                return;
            }
        } else {
            if (canDisplace(x, y, x + 1, y + 1)) {
                swapCells(x + 1, y + 1, x, y);
                return;
            }
            if (canDisplace(x, y, x - 1, y + 1)) {
                swapCells(x - 1, y + 1, x, y);
                return;
            }
        }
    }

    void updateLiquid(int x, int y) {
        //check directly below
        if (canDisplace(x, y, x, y + 1)) {
            swapCells(x, y, x, y + 1);
            return;
        }

        //randomize checking left or right first to add some variance to liquid movements
        bool leftFirst = rand() % 2 == 0;

        if (leftFirst) {
            if (canDisplace(x, y, x - 1, y + 1)) {
                swapCells(x - 1, y + 1, x, y);
                return;
            }
            if (canDisplace(x, y, x + 1, y + 1)) {
                swapCells(x + 1, y + 1, x, y);
                return;
            }
        } else {
            if (canDisplace(x, y, x + 1, y + 1)) {
                swapCells(x + 1, y + 1, x, y);
                return;
            }
            if (canDisplace(x, y, x - 1, y + 1)) {
                swapCells(x - 1, y + 1, x, y);
                return;
            }
        }
        //if diagonals dont work, move sideways
        if (leftFirst) {
            if (canDisplace(x, y, x - 1, y)) {
                swapCells(x - 1, y, x, y);
                return;
            }
            if (canDisplace(x, y, x + 1, y)) {
                swapCells(x + 1, y, x, y);
                return;
            }
        } else {
            if (canDisplace(x, y, x + 1, y)) {
                swapCells(x + 1, y, x, y);
                return;
            }
            if (canDisplace(x, y, x - 1, y)) {
                swapCells(x - 1, y, x, y);
                return;
            }
        }
    }

    void updateGas(int x, int y) {
        //check directly below
        if (canDisplace(x, y, x, y - 1)) {
            swapCells(x, y, x, y - 1);
            return;
        }

        //randomize checking left or right first to add some variance to gas movements
        bool leftFirst = rand() % 2 == 0;

        if (leftFirst) {
            if (canDisplace(x, y, x - 1, y - 1)) {
                swapCells(x - 1, y - 1, x, y);
                return;
            }
            if (canDisplace(x, y, x + 1, y - 1)) {
                swapCells(x + 1, y - 1, x, y);
                return;
            }
        } else {
            if (canDisplace(x, y, x + 1, y - 1)) {
                swapCells(x + 1, y - 1, x, y);
                return;
            }
            if (canDisplace(x, y, x - 1, y - 1)) {
                swapCells(x - 1, y - 1, x, y);
                return;
            }
        }
        //if diagonals dont work, move sideways
        if (leftFirst) {
            if (canDisplace(x, y, x - 1, y)) {
                swapCells(x - 1, y, x, y);
                return;
            }
            if (canDisplace(x, y, x + 1, y)) {
                swapCells(x + 1, y, x, y);
                return;
            }
        } else {
            if (canDisplace(x, y, x + 1, y)) {
                swapCells(x + 1, y, x, y);
                return;
            }
            if (canDisplace(x, y, x - 1, y)) {
                swapCells(x - 1, y, x, y);
                return;
            }
        }
    }

    void updateStatic(int x, int y) {
        //shouldnt move, so nothing
    }

    bool spawnParticleAtCell(int gx, int gy, ParticleType type, Entity &entity) {
        if (!inBounds(gx, gy) || !isEmpty(gx, gy)) return false;
        Cell &cell = at(gx, gy);
        cell.type = type;
        cell.entity = &entity;
        cell.behaviour = entity.getComponent<ParticleProperties>().behaviour;
        // std::cout << "spawnParticleAtCell: gx=" << gx
        //   << " gy=" << gy
        //   << " type=" << static_cast<int>(type)
        //   << std::endl;
        return true;
    };

    bool canDisplace(int fromX, int fromY, int toX, int toY) {
        if (!inBounds(toX, toY)) return false;
        ParticleType mover = at(fromX, fromY).type;
        ParticleType target = at(toX, toY).type;

        if (ParticleType::Empty == target) return true;

        return ParticleHelpers::getProperties(mover).density > ParticleHelpers::getProperties(target).density;
    }

private
:
    int width;
    int height;
    int cellSize;
    std::vector<Cell> cells;
};

#endif //INC_8051TUTORIAL_PARTICLEGRIDSYSTEM_H
