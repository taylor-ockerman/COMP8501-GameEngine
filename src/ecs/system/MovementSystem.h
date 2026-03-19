//
// Created by taylo on 1/21/2026.
//

#ifndef INC_8051TUTORIAL_MOVEMENTSYSTEM_H
#define INC_8051TUTORIAL_MOVEMENTSYSTEM_H
#include <memory>
#include <vector>
#include "../Entity.h"
#include "../Component.h"

class MovementSystem {
public:
    void update(std::vector<std::unique_ptr<Entity> > &entities, float dt) {
        for (auto &entity: entities) {
            if (entity->hasComponent<Transform>() && entity->hasComponent<Velocity>()) {
                //t.position.x += 60 * dt;
                //t.position.y += 60 * dt;
                auto &t = entity->getComponent<Transform>();
                auto &v = entity->getComponent<Velocity>();
                //track previous frames position
                t.oldPosition = t.position;

                Vector2D directionVec = v.direction;

                //normalizing
                directionVec.normalize();

                //Vector2D needs an operator function to multiply a float
                Vector2D velocityVec = directionVec * v.magnitude;
                //Vector2D velocityVec = directionVec;
                t.position += velocityVec * dt;
            }
        }
    }
};
#endif //INC_8051TUTORIAL_MOVEMENTSYSTEM_H
