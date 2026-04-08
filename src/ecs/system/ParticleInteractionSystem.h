//
// Created by taylo on 3/30/2026.
//

#ifndef INC_8051TUTORIAL_PARTICLEINTERACTIONSYSTEM_H
#define INC_8051TUTORIAL_PARTICLEINTERACTIONSYSTEM_H

#include <memory>
#include <vector>

#include "Entity.h"
#include "ParticleGrid.h"
#include "ParticleHelpers.h"
class World;

class ParticleInteractionSystem {
public:
    void update(World &world, std::vector<std::unique_ptr<Entity> > &entities, ParticleGrid &grid);

private:
    void handleFire(World &world, Entity *entity, ParticleGrid &grid);

    void handleSmoke(Entity *entity, ParticleGrid &grid);

    void handleSteam(Entity *entity, ParticleGrid &grid);

    void tickParticleLife(Entity *entity, ParticleGrid &grid, ParticleType expectedType);

    void destroyParticle(Entity *entity, ParticleGrid &grid);

    void convertParticle(Entity *entity, Cell &cell, ParticleType newType, int gx, int gy);
};

#endif //INC_8051TUTORIAL_PARTICLEINTERACTIONSYSTEM_H
