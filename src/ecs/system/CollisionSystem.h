//
// Created by taylo on 2/4/2026.
//

#ifndef INC_8051TUTORIAL_COLLISIONSYSTEM_H
#define INC_8051TUTORIAL_COLLISIONSYSTEM_H
#include <algorithm>
#include <memory>
#include <set>
#include <vector>
#include "../Entity.h"

using CollisionKey = std::pair<Entity *, Entity *>;
//forward declaration
class World;

class CollisionSystem {
public:
    void update(World &world);

    //void freeCollision(Entity *entityA, Entity *entityB);
    std::set<CollisionKey> activeCollisions;

private:
    std::vector<Entity *> queryCollidables(const std::vector<std::unique_ptr<Entity> > &entities);

    CollisionKey makeKey(Entity *entityA, Entity *entityB) {
        return std::minmax(entityA, entityB);
        //automatically orders our pair so smaller number (mem address) is ordered first
    }
};

#endif //INC_8051TUTORIAL_COLLISIONSYSTEM_H
