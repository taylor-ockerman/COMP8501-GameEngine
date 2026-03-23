//
// Created by taylo on 3/17/2026.
//

#ifndef INC_8051TUTORIAL_GRAVITYSYSTEM_H
#define INC_8051TUTORIAL_GRAVITYSYSTEM_H

#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include "Component.h"
#include "Entity.h"

class GravitySystem {
public:
    void update(std::vector<std::unique_ptr<Entity> > &entities, float gravity, float dt) {
        for (auto &e: entities) {
            if (e->hasComponent<Particle>()) continue;
            if (e->hasComponent<Transform>() && e->hasComponent<Velocity>() && e->hasComponent<Acceleration>()) {
                auto &velocity = e->getComponent<Velocity>();
                auto &acceleration = e->getComponent<Acceleration>();
                if (acceleration.isGrounded) {
                    acceleration.magnitude = 0.0f;
                } else {
                    acceleration.magnitude = gravity;
                }
                //create new vector that represents the velocity and direction of the entity
                Vector2D velVector{
                    velocity.direction.x * velocity.magnitude,
                    velocity.direction.y * velocity.magnitude
                };
                //add the force of gravity to the y component
                velVector.y += acceleration.magnitude;
                //calculate the magnitude of the new vector after gravity
                float mag = std::sqrt(velVector.x * velVector.x + velVector.y * velVector.y);

                velocity.direction = velVector.normalize();
                velocity.magnitude = mag;
            }
        }
    }
};

#endif //INC_8051TUTORIAL_GRAVITYSYSTEM_H
