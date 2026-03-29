//
// Created by taylo on 3/29/2026.
//

#ifndef INC_8051TUTORIAL_PARTICLESIMULATIONSYSTEM_H
#define INC_8051TUTORIAL_PARTICLESIMULATIONSYSTEM_H
#include "ParticleGrid.h"

class ParticleGrid;

class ParticleSimulationSystem {
public:
    void update(ParticleGrid &grid) {
        for (int y = grid.getHeight() - 1; y >= 0; y--) {
            bool randomRowDirection = rand() % 2 == 0;
            if (randomRowDirection) {
                for (int x = 0; x < grid.getWidth(); x++) {
                    {
                        ParticleBehaviour bh = grid.at(x, y).behaviour;
                        if (bh == ParticleBehaviour::Gas) continue;
                        switch (bh) {
                            case ParticleBehaviour::Powder:
                                updatePowder(grid, x, y);
                                //std::cout << "updating sand" << std::endl;
                                break;
                            case ParticleBehaviour::Liquid:
                                updateLiquid(grid, x, y);
                                //std::cout << "updating water"<< std::endl;
                                break;
                            case ParticleBehaviour::Static:
                                updateStatic(grid, x, y);
                                break;
                            default:
                                break;
                        }
                    }
                }
            } else {
                for (int x = grid.getWidth() - 1; x >= 0; x--) {
                    {
                        ParticleBehaviour bh = grid.at(x, y).behaviour;
                        if (bh == ParticleBehaviour::Gas) continue;
                        switch (bh) {
                            case ParticleBehaviour::Powder:
                                updatePowder(grid, x, y);
                                //std::cout << "updating sand" << std::endl;
                                break;
                            case ParticleBehaviour::Liquid:
                                updateLiquid(grid, x, y);
                                //std::cout << "updating water"<< std::endl;
                                break;
                            case ParticleBehaviour::Static:
                                updateStatic(grid, x, y);
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }
        for (int y = 0; y < grid.getHeight(); y++) {
            bool randomRowDirection = rand() % 2 == 0;
            if (randomRowDirection) {
                for (int x = 0; x < grid.getWidth(); x++) {
                    if (grid.at(x, y).behaviour == ParticleBehaviour::Gas
                    ) {
                        updateGas(grid, x, y);
                    }
                }
            } else {
                for (int x = grid.getWidth() - 1; x >= 0; x--) {
                    if (grid.at(x, y).behaviour == ParticleBehaviour::Gas
                    ) {
                        updateGas(grid, x, y);
                    }
                }
            }
        }
    }

private:
    void updatePowder(ParticleGrid &grid, int x, int y) {
        //check directly below
        if (canDisplace(grid, x, y, x, y + 1)) {
            grid.swapCells(x, y, x, y + 1);
            return;
        }

        //randomize checking left or right first to add some variance to powder movements
        bool leftFirst = rand() % 2 == 0;

        if (leftFirst) {
            if (canDisplace(grid, x, y, x - 1, y + 1)) {
                grid.swapCells(x - 1, y + 1, x, y);
                return;
            }
            if (canDisplace(grid, x, y, x + 1, y + 1)) {
                grid.swapCells(x + 1, y + 1, x, y);
                return;
            }
        } else {
            if (canDisplace(grid, x, y, x + 1, y + 1)) {
                grid.swapCells(x + 1, y + 1, x, y);
                return;
            }
            if (canDisplace(grid, x, y, x - 1, y + 1)) {
                grid.swapCells(x - 1, y + 1, x, y);
                return;
            }
        }
    }

    void updateLiquid(ParticleGrid &grid, int x, int y) {
        //check directly below
        if (canDisplace(grid, x, y, x, y + 1)) {
            grid.swapCells(x, y, x, y + 1);
            return;
        }

        //randomize checking left or right first to add some variance to liquid movements
        bool leftFirst = rand() % 2 == 0;

        if (leftFirst) {
            if (canDisplace(grid, x, y, x - 1, y + 1)) {
                grid.swapCells(x - 1, y + 1, x, y);
                return;
            }
            if (canDisplace(grid, x, y, x + 1, y + 1)) {
                grid.swapCells(x + 1, y + 1, x, y);
                return;
            }
        } else {
            if (canDisplace(grid, x, y, x + 1, y + 1)) {
                grid.swapCells(x + 1, y + 1, x, y);
                return;
            }
            if (canDisplace(grid, x, y, x - 1, y + 1)) {
                grid.swapCells(x - 1, y + 1, x, y);
                return;
            }
        }
        //if diagonals dont work, move sideways
        if (leftFirst) {
            if (canDisplace(grid, x, y, x - 1, y)) {
                grid.swapCells(x - 1, y, x, y);
                return;
            }
            if (canDisplace(grid, x, y, x + 1, y)) {
                grid.swapCells(x + 1, y, x, y);
                return;
            }
        } else {
            if (canDisplace(grid, x, y, x + 1, y)) {
                grid.swapCells(x + 1, y, x, y);
                return;
            }
            if (canDisplace(grid, x, y, x - 1, y)) {
                grid.swapCells(x - 1, y, x, y);
                return;
            }
        }
    }

    void updateGas(ParticleGrid &grid, int x, int y) {
        //check directly below
        if (canDisplace(grid, x, y, x, y - 1)) {
            grid.swapCells(x, y, x, y - 1);
            return;
        }

        //randomize checking left or right first to add some variance to gas movements
        bool leftFirst = rand() % 2 == 0;

        if (leftFirst) {
            if (canDisplace(grid, x, y, x - 1, y - 1)) {
                grid.swapCells(x - 1, y - 1, x, y);
                return;
            }
            if (canDisplace(grid, x, y, x + 1, y - 1)) {
                grid.swapCells(x + 1, y - 1, x, y);
                return;
            }
        } else {
            if (canDisplace(grid, x, y, x + 1, y - 1)) {
                grid.swapCells(x + 1, y - 1, x, y);
                return;
            }
            if (canDisplace(grid, x, y, x - 1, y - 1)) {
                grid.swapCells(x - 1, y - 1, x, y);
                return;
            }
        }
        //if diagonals dont work, move sideways
        if (leftFirst) {
            if (canDisplace(grid, x, y, x - 1, y)) {
                grid.swapCells(x - 1, y, x, y);
                return;
            }
            if (canDisplace(grid, x, y, x + 1, y)) {
                grid.swapCells(x + 1, y, x, y);
                return;
            }
        } else {
            if (canDisplace(grid, x, y, x + 1, y)) {
                grid.swapCells(x + 1, y, x, y);
                return;
            }
            if (canDisplace(grid, x, y, x - 1, y)) {
                grid.swapCells(x - 1, y, x, y);
                return;
            }
        }
    }

    void updateStatic(ParticleGrid &grid, int x, int y) {
        //shouldnt move, so nothing
    }

    bool canDisplace(ParticleGrid &grid, int fromX, int fromY, int toX, int toY) {
        if (!grid.inBounds(toX, toY)) return false;
        ParticleType mover = grid.at(fromX, fromY).type;
        ParticleType target = grid.at(toX, toY).type;

        if (ParticleType::Empty == target) return true;

        return ParticleHelpers::getProperties(mover).density > ParticleHelpers::getProperties(target).density;
    }
};
#endif //INC_8051TUTORIAL_PARTICLESIMULATIONSYSTEM_H
