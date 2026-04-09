//
// Created by taylo on 4/5/2026.
//
#include "ParticleSpawnDeSpawnSystem.h"
#include "World.h"

void ParticleSpawnDeSpawnSystem::update(World &world, ParticleGrid &grid) {
    BrushState *brushState = nullptr;

    for (auto &e: world.getEntities()) {
        if (e->hasComponent<BrushState>()) {
            brushState = &e->getComponent<BrushState>();
            break;
        }
    }

    if (brushState == nullptr) return;
    if (!brushState->isPainting) return;
    if (brushState->uiCapturedClick) return;

    int worldX = static_cast<int>(brushState->mouseWorldPos.x);
    int worldY = static_cast<int>(brushState->mouseWorldPos.y);

    if (brushState->selectedParticle == ParticleType::Erase) {
        eraseBrushAtWorld(world, grid, worldX, worldY, brushState->brushSize);
    } else if (brushState->selectedParticle != ParticleType::Empty) {
        spawnBrushAtWorld(world, grid, worldX, worldY,
                          brushState->selectedParticle,
                          brushState->brushSize);
    }
}

//calls function to spawn particles at each brush coordinate
void ParticleSpawnDeSpawnSystem::spawnBrushAtWorld(World &world, ParticleGrid &grid,
                                                   int worldX, int worldY,
                                                   ParticleType type, int brushSize) {
    int cellSize = grid.getCellSize();

    for (int dy = -brushSize; dy <= brushSize; dy++) {
        for (int dx = -brushSize; dx <= brushSize; dx++) {
            if (dx * dx + dy * dy > brushSize * brushSize) continue;

            spawnParticleAtWorld(world,
                                 grid,
                                 worldX + dx * cellSize,
                                 worldY + dy * cellSize,
                                 type);
        }
    }
}

//convert world coords into grid coords and then calls func to spawn particles at those cells
bool ParticleSpawnDeSpawnSystem::spawnParticleAtWorld(World &world, ParticleGrid &grid,
                                                      int worldX, int worldY,
                                                      ParticleType type) {
    int cellSize = grid.getCellSize();
    int gx = worldX / cellSize;
    int gy = worldY / cellSize;

    if (!grid.inBounds(gx, gy)) return false;
    if (grid.at(gx, gy).occupiedByPlayer) return false;
    return spawnParticleAtCell(world, grid, gx, gy, type);
}

//the function that does the actual spawning/particle replacement
bool ParticleSpawnDeSpawnSystem::spawnParticleAtCell(World &world, ParticleGrid &grid,
                                                     int gx, int gy,
                                                     ParticleType type) {
    if (!grid.inBounds(gx, gy)) return false;

    Cell &cell = grid.at(gx, gy);

    if (cell.occupiedByPlayer) return false;
    if (cell.type == ParticleType::Wall) return false;

    if (!grid.isEmpty(gx, gy)) {
        int newDensity = ParticleHelpers::getProperties(type, false).density;
        int oldDensity = ParticleHelpers::getProperties(cell.type, false).density;

        if (newDensity <= oldDensity || cell.type == ParticleType::Wall) return false;

        if (cell.entity != nullptr) {
            cell.entity->destroy();
        }

        grid.clearCell(gx, gy);
    }

    int cellSize = grid.getCellSize();

    auto &e = world.createDeferredEntity();
    float worldX = static_cast<float>(gx * cellSize);
    float worldY = static_cast<float>(gy * cellSize);

    e.addComponent<Transform>(Vector2D(worldX, worldY), 0.0f, 1.0f, Vector2D(gx, gy));
    e.addComponent<Particle>(type, gx, gy);

    static SDL_Texture *tex = TextureManager::load("assets/particle_tileset.png");
    e.addComponent<ParticleProperties>(ParticleHelpers::getProperties(type, true));
    e.getComponent<Particle>().life = e.getComponent<ParticleProperties>().life;

    SDL_FRect dest{worldX, worldY, static_cast<float>(cellSize), static_cast<float>(cellSize)};
    e.addComponent<Sprite>(tex, e.getComponent<ParticleProperties>().spriteSrc, dest,
                           RenderLayer::World, true);

    cell.type = type;
    cell.entity = &e;
    cell.behaviour = e.getComponent<ParticleProperties>().behaviour;

    grid.wakeChunkAndNeighborsForCell(gx, gy);
    grid.getChunkFromCell(gx, gy).movedThisFrame = true;
    return true;
}

//does actual erasing
void ParticleSpawnDeSpawnSystem::eraseBrushAtWorld(World &world, ParticleGrid &grid,
                                                   int worldX, int worldY,
                                                   int brushSize) {
    int cellSize = grid.getCellSize();
    int centerGX = worldX / cellSize;
    int centerGY = worldY / cellSize;

    for (int dy = -brushSize; dy <= brushSize; dy++) {
        for (int dx = -brushSize; dx <= brushSize; dx++) {
            if (dx * dx + dy * dy > brushSize * brushSize) continue;

            int gx = centerGX + dx;
            int gy = centerGY + dy;

            if (!grid.inBounds(gx, gy)) continue;

            Cell &cell = grid.at(gx, gy);
            if (cell.type == ParticleType::Wall) continue;

            grid.clearCell(gx, gy);
            grid.wakeChunkAndNeighborsForCell(gx, gy);
            grid.getChunkFromCell(gx, gy).movedThisFrame = true;
        }
    }
}

void ParticleSpawnDeSpawnSystem::destroyAllParticles(World &world, ParticleGrid *grid) {
    if (grid != nullptr) {
        grid->clearGrid();
    }

    for (auto &entity: world.getEntities()) {
        if (entity->hasComponent<Particle>()) {
            entity->destroy();
        }
    }

    for (auto &entity: world.getDeferredEntities()) {
        if (entity->hasComponent<Particle>()) {
            entity->destroy();
        }
    }

    world.cleanup();
}

void ParticleSpawnDeSpawnSystem::destroyParticle(ParticleGrid &grid, int gx, int gy) {
    if (!grid.inBounds(gx, gy)) return;

    Cell &cell = grid.at(gx, gy);

    grid.clearCell(gx, gy);
    grid.wakeChunkAndNeighborsForCell(gx, gy);
    grid.getChunkFromCell(gx, gy).movedThisFrame = true;
}
