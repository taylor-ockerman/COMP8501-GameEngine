//
// Created by taylo on 2/4/2026.
//

#ifndef INC_8051TUTORIAL_COLLISIONSYSTEM_H
#define INC_8051TUTORIAL_COLLISIONSYSTEM_H
#include <memory>
#include <vector>
#include "../Entity.h"
//forward declaration
class World;

class CollisionSystem {
public:
    void update(World &world);

    void freeCollision(Entity *entityA, Entity *entityB);

private:
    std::vector<Entity *> queryCollidables(const std::vector<std::unique_ptr<Entity> > &entities);
};

#endif //INC_8051TUTORIAL_COLLISIONSYSTEM_H
