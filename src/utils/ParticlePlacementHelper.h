//
// Created by taylo on 3/29/2026.
//

#ifndef INC_8051TUTORIAL_PARTICLEPLACEMENTSYSTEM_H
#define INC_8051TUTORIAL_PARTICLEPLACEMENTSYSTEM_H
#include "Component.h"
#include "ParticleGrid.h"

class World;

class ParticlePlacementHelper {
public:
    //fills cells in the grid based on wall collider rect geometry, so we can avoid tons of collision events between particles and walls.
    static void addWallRect(ParticleGrid &grid, const SDL_FRect &wallRect) {
        int cellSize = grid.getCellSize();
        int startGX = static_cast<int>(wallRect.x) / cellSize;
        int endGX = static_cast<int>(wallRect.x + wallRect.w - 1) / cellSize;
        int startGY = static_cast<int>(wallRect.y) / cellSize;
        int endGY = static_cast<int>(wallRect.y + wallRect.h - 1) / cellSize;

        for (int gy = startGY; gy <= endGY; gy++) {
            for (int gx = startGX; gx <= endGX; gx++) {
                if (!grid.inBounds(gx, gy)) continue;
                Cell &cell = grid.at(gx, gy);
                cell.type = ParticleType::Wall;
                cell.behaviour = ParticleBehaviour::Static;
                cell.entity = nullptr;
            }
        }
    }

private:
};
#endif //INC_8051TUTORIAL_PARTICLEPLACEMENTSYSTEM_H
