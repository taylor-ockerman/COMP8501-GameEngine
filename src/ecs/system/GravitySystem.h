//
// Created by taylo on 3/17/2026.
//

#ifndef INC_8051TUTORIAL_GRAVITYSYSTEM_H
#define INC_8051TUTORIAL_GRAVITYSYSTEM_H

#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class GravitySystem {
public:
    void update(std::vector<std::unique_ptr<Entity> > &entities, float dt) {
        for (auto &e: entities) {
            if (e->hasComponent<Transform>() && e->hasComponent<Velocity>()) {
                auto &velocity = e->getComponent<Velocity>();
                Vector2D dir = velocity.direction;
                dir += gravity;
                dir.normalize();
                velocity.direction = dir;
            }
        }
    }

private:
    Vector2D gravity{0.01f, 0.01f};
};

#endif //INC_8051TUTORIAL_GRAVITYSYSTEM_H
