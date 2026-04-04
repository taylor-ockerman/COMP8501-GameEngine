//
// Created by taylo on 3/29/2026.
//

#ifndef INC_8051TUTORIAL_PARTICLESIMULATIONSYSTEM_H
#define INC_8051TUTORIAL_PARTICLESIMULATIONSYSTEM_H
#include <queue>

#include "ParticleGrid.h"

class ParticleGrid;

class ParticleSimulationSystem {
public:
    void update(ParticleGrid &grid, std::vector<std::unique_ptr<Entity> > &entities) {
        beginChunkFrame(grid);
        clearPlayerOccupancy(grid);
        markPlayerOccupancy(grid, entities);
        // powders/liquids/static: bottom-up chunk pass
        for (int cy = grid.getChunkHeight() - 1; cy >= 0; cy--) {
            bool leftToRightChunks = rand() % 2 == 0;

            if (leftToRightChunks) {
                for (int cx = 0; cx < grid.getChunkWidth(); cx++) {
                    updateChunkNonGas(grid, cx, cy);
                }
            } else {
                for (int cx = grid.getChunkWidth() - 1; cx >= 0; cx--) {
                    updateChunkNonGas(grid, cx, cy);
                }
            }
        }

        // gas: top-down chunk pass
        for (int cy = 0; cy < grid.getChunkHeight(); cy++) {
            bool leftToRightChunks = rand() % 2 == 0;

            if (leftToRightChunks) {
                for (int cx = 0; cx < grid.getChunkWidth(); cx++) {
                    updateChunkGas(grid, cx, cy);
                }
            } else {
                for (int cx = grid.getChunkWidth() - 1; cx >= 0; cx--) {
                    updateChunkGas(grid, cx, cy);
                }
            }
        }
        endChunkFrame(grid);
    }

    void beginChunkFrame(ParticleGrid &grid) {
        for (auto &chunk: grid.getChunks()) {
            chunk.movedThisFrame = false;
        }
    }

    void endChunkFrame(ParticleGrid &grid) {
        for (auto &chunk: grid.getChunks()) {
            if (chunk.movedThisFrame) {
                chunk.active = true;
                chunk.idleFrames = 0;
            } else if (chunk.active) {
                chunk.idleFrames++;
                if (chunk.idleFrames >= 3) {
                    chunk.active = false;
                }
            }
        }
    }

private:
    void updateChunkNonGas(ParticleGrid &grid, int cx, int cy) {
        if (!grid.inBoundsChunk(cx, cy)) return;

        Chunk &chunk = grid.chunkAt(cx, cy);
        if (!chunk.active) return;
        // std::cout << "chunk " << cx << "," << cy
        //         << " active=" << chunk.active
        //         << " idle=" << chunk.idleFrames
        //         << " moved=" << chunk.movedThisFrame << std::endl;

        int chunkSize = grid.getChunkSize();

        int startX = cx * chunkSize;
        int endX = std::min(startX + chunkSize, grid.getWidth());

        int startY = cy * chunkSize;
        int endY = std::min(startY + chunkSize, grid.getHeight());

        for (int y = endY - 1; y >= startY; y--) {
            bool leftToRight = rand() % 2 == 0;

            if (leftToRight) {
                for (int x = startX; x < endX; x++) {
                    ParticleBehaviour bh = grid.at(x, y).behaviour;
                    if (bh == ParticleBehaviour::Gas) continue;

                    switch (bh) {
                        case ParticleBehaviour::Powder:
                            updatePowder(grid, x, y);
                            break;
                        case ParticleBehaviour::Liquid:
                            updateLiquid(grid, x, y);
                            break;
                        case ParticleBehaviour::Static:
                            updateStatic(grid, x, y);
                            break;
                        default:
                            break;
                    }
                }
            } else {
                for (int x = endX - 1; x >= startX; x--) {
                    ParticleBehaviour bh = grid.at(x, y).behaviour;
                    if (bh == ParticleBehaviour::Gas) continue;

                    switch (bh) {
                        case ParticleBehaviour::Powder:
                            updatePowder(grid, x, y);
                            break;
                        case ParticleBehaviour::Liquid:
                            updateLiquid(grid, x, y);
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

    void updateChunkGas(ParticleGrid &grid, int cx, int cy) {
        if (!grid.inBoundsChunk(cx, cy)) return;

        Chunk &chunk = grid.chunkAt(cx, cy);
        if (!chunk.active) return;
        // std::cout << "chunk " << cx << "," << cy
        //         << " active=" << chunk.active
        //         << " idle=" << chunk.idleFrames
        //         << " moved=" << chunk.movedThisFrame << std::endl;

        int chunkSize = grid.getChunkSize();

        int startX = cx * chunkSize;
        int endX = std::min(startX + chunkSize, grid.getWidth());

        int startY = cy * chunkSize;
        int endY = std::min(startY + chunkSize, grid.getHeight());

        for (int y = startY; y < endY; y++) {
            bool leftToRight = rand() % 2 == 0;

            if (leftToRight) {
                for (int x = startX; x < endX; x++) {
                    if (grid.at(x, y).behaviour == ParticleBehaviour::Gas) {
                        updateGas(grid, x, y);
                    }
                }
            } else {
                for (int x = endX - 1; x >= startX; x--) {
                    if (grid.at(x, y).behaviour == ParticleBehaviour::Gas) {
                        updateGas(grid, x, y);
                    }
                }
            }
        }
    }


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
        if (grid.at(toX, toY).occupiedByPlayer) return false;
        ParticleType mover = grid.at(fromX, fromY).type;
        ParticleType target = grid.at(toX, toY).type;
        if (ParticleType::Empty == target) return true;

        return ParticleHelpers::getProperties(mover, false).density > ParticleHelpers::getProperties(target, false).
               density;
    }

    void clearPlayerOccupancy(ParticleGrid &grid) {
        for (int y = 0; y < grid.getHeight(); y++) {
            for (int x = 0; x < grid.getWidth(); x++) {
                grid.at(x, y).occupiedByPlayer = false;
            }
        }
    }

    static void markPlayerOccupancy(ParticleGrid &grid, std::vector<std::unique_ptr<Entity> > &entities) {
        int radius = 4;

        for (auto &e: entities) {
            if (!e->hasComponent<PlayerTag>() || !e->hasComponent<Collider>()) continue;

            auto &collider = e->getComponent<Collider>();

            int playerCenterX = grid.worldToGridX(collider.rect.x + collider.rect.w / 2);
            int playerCenterY = grid.worldToGridY(collider.rect.y + collider.rect.h / 2);

            for (int dy = -radius; dy <= radius; dy++) {
                for (int dx = -radius; dx <= radius; dx++) {
                    if (dx * dx + dy * dy > radius * radius) continue;

                    int gx = playerCenterX + dx;
                    int gy = playerCenterY + dy;

                    if (!grid.inBounds(gx, gy)) continue;

                    Cell &cell = grid.at(gx, gy);
                    cell.occupiedByPlayer = true;
                    grid.getPreviousPlayerCells().push_back({gx, gy});

                    if (cell.type != ParticleType::Wall) {
                        grid.clearCell(gx, gy);
                        cell.occupiedByPlayer = true;
                    }

                    grid.wakeChunkAndNeighborsForCell(gx, gy);
                    grid.getChunkFromCell(gx, gy).movedThisFrame = true;
                }
            }
        }
    }

    void clearPreviousPlayerOccupancy(ParticleGrid &grid) {
        auto &cells = grid.getPreviousPlayerCells();
        for (const auto &[x, y]: cells) {
            if (!grid.inBounds(x, y)) continue;
            grid.at(x, y).occupiedByPlayer = false;
        }
        cells.clear();
    }
};
#endif //INC_8051TUTORIAL_PARTICLESIMULATIONSYSTEM_H
