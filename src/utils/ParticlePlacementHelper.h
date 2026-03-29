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
    static bool spawnParticleAtCell(ParticleGrid &grid, int gx, int gy, ParticleType type, Entity &entity);

    //fills cells in the grid based on wall collider rect geometry, so we can avoid tons of collision events between particles and walls.
    static void addWallRect(ParticleGrid &grid, const SDL_FRect &wallRect);

    static void spawnBrushAtWorld(World &world, int worldX, int worldY, ParticleGrid &grid, ParticleType type);

private:
};
#endif //INC_8051TUTORIAL_PARTICLEPLACEMENTSYSTEM_H
