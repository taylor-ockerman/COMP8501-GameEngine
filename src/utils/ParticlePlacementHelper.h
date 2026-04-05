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
    static void addWallRect(ParticleGrid &grid, const SDL_FRect &wallRect);

private:
};
#endif //INC_8051TUTORIAL_PARTICLEPLACEMENTSYSTEM_H
