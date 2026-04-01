//
// Created by taylo on 3/30/2026.
//

#ifndef INC_8051TUTORIAL_PARTICLEINTERACTIONSYSTEM_H
#define INC_8051TUTORIAL_PARTICLEINTERACTIONSYSTEM_H

#include <cmath>
#include <memory>
#include <vector>

#include "Collision.h"
#include "Entity.h"
#include "ParticleGrid.h"

class ParticleInteractionSystem {
public:
    void update(std::vector<std::unique_ptr<Entity> > &entities, ParticleGrid &grid) {
    }
};

#endif //INC_8051TUTORIAL_PARTICLEINTERACTIONSYSTEM_H
