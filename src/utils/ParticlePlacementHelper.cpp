//
// Created by taylo on 3/29/2026.
//
#include "ParticlePlacementHelper.h"

#include "World.h"

bool ParticlePlacementHelper::
spawnParticleAtCell(ParticleGrid &grid, int gx, int gy, ParticleType type, Entity &entity) {
    if (!grid.inBounds(gx, gy) || !grid.isEmpty(gx, gy)) return false;
    Cell &cell = grid.at(gx, gy);
    cell.type = type;
    cell.entity = &entity;
    cell.behaviour = entity.getComponent<ParticleProperties>().behaviour;

    grid.wakeChunkAndNeighborsForCell(gx, gy);
    grid.getChunkFromCell(gx, gy).movedThisFrame = true;
    // std::cout << "spawnParticleAtCell: gx=" << gx
    //   << " gy=" << gy
    //   << " type=" << static_cast<int>(type)
    //   << std::endl;
    return true;
}

//fills cells in the grid based on wall collider rect geometry, so we can avoid tons of collision events between particles and walls.
void ParticlePlacementHelper::addWallRect(ParticleGrid &grid, const SDL_FRect &wallRect) {
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
    //maybe add chunk wake??
}

void ParticlePlacementHelper::spawnBrushAtWorld(World &world, int worldX, int worldY, ParticleGrid &grid,
                                                ParticleType type) {
    int cellSize = grid.getCellSize();
    int brushSize = world.getBrushSize();
    for (int dy = -brushSize; dy <= brushSize; dy++) {
        for (int dx = -brushSize; dx <= brushSize; dx++) {
            if (dx * dx + dy * dy > brushSize * brushSize) continue;
            world.spawnParticleAtWorld(worldX + dx * cellSize, worldY + dy * cellSize, grid, type);
        }
    }
}
