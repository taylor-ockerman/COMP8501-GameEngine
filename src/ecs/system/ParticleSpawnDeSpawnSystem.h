//
// Created by taylo on 4/5/2026.
//

#ifndef INC_8051TUTORIAL_PARTICLECREATIONSYSTEM_H
#define INC_8051TUTORIAL_PARTICLECREATIONSYSTEM_H
#include "ParticleGrid.h"
class World;

class ParticleSpawnDeSpawnSystem {
public:
    void update(World &world, ParticleGrid &grid);

    //ParticlePlacementHelpers functions
    // bool spawnParticleAtCell(ParticleGrid &grid, int gx, int gy, ParticleType type, Entity &entity);

    //void spawnBrushAtWorld(World &world, int worldX, int worldY, ParticleGrid &grid, ParticleType type);

    //World functions
private:
    bool spawnParticleAtWorld(World &world, ParticleGrid &grid,
                              int worldX, int worldY,
                              ParticleType type);

    void spawnBrushAtWorld(World &world, ParticleGrid &grid,
                           int worldX, int worldY,
                           ParticleType type, int brushSize);

    bool spawnParticleAtCell(World &world, ParticleGrid &grid,
                             int gx, int gy,
                             ParticleType type);

    void eraseBrushAtWorld(World &world, ParticleGrid &grid,
                           int worldX, int worldY,
                           int brushSize);

    void destroyParticle(ParticleGrid &grid, int gx, int gy);

    void destroyAllParticles(World &world, ParticleGrid *grid);
};

#endif //INC_8051TUTORIAL_PARTICLECREATIONSYSTEM_H
