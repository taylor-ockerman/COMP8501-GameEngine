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
                velocity.direction += gravity * dt;
            }
        }
    }

private:
    Vector2D gravity = Vector2D(0.0f, .3f);
};

#endif //INC_8051TUTORIAL_GRAVITYSYSTEM_H
