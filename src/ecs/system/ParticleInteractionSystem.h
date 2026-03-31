//
// Created by taylo on 3/30/2026.
//

#ifndef INC_8051TUTORIAL_PARTICLEINTERACTIONSYSTEM_H
#define INC_8051TUTORIAL_PARTICLEINTERACTIONSYSTEM_H
#include <memory>
#include <vector>

#include "Collision.h"
#include "CollisionResolution.h"
#include "Entity.h"
#include "ParticleGrid.h"

class ParticleInteractionSystem {
public:
    void update(std::vector<std::unique_ptr<Entity> > &entities, ParticleGrid &grid) {
        int cellSize = grid.getCellSize();
        for (auto &entity: entities) {
            if (!entity->hasComponent<PlayerTag>() || !entity->hasComponent<Collider>()) continue;
            auto &collider = entity->getComponent<Collider>();

            int startGX = grid.worldToGridX(collider.rect.x);
            int startGY = grid.worldToGridY(collider.rect.y);
            int endGX = grid.worldToGridX(collider.rect.x + collider.rect.w - 1);
            int endGY = grid.worldToGridY(collider.rect.y + collider.rect.h - 1);

            bool touchingSolidParticle = false;
            //bool touchingWater = false;
            //bool touchingSmoke = false;
            if (entity->hasComponent<Acceleration>()) {
                entity->getComponent<Acceleration>().isGrounded = false;
            }
            for (int gy = startGY; gy <= endGY; gy++) {
                for (int gx = startGX; gx <= endGX; gx++) {
                    if (!grid.inBounds(gx, gy)) continue;

                    Cell &cell = grid.at(gx, gy);
                    if (cell.type != ParticleType::Sand && cell.type != ParticleType::Stone) continue;

                    SDL_FRect cellRect{
                        static_cast<float>(gx * cellSize),
                        static_cast<float>(gy * cellSize),
                        static_cast<float>(cellSize),
                        static_cast<float>(cellSize)
                    };

                    if (Collision::AABB(collider.rect, cellRect)) {
                        touchingSolidParticle = true;
                        CollisionResolution::resolvePlayerSolidParticle(*entity, cellRect);
                    }
                }
            }

            handlePlayerParticleEffects(*entity, touchingSolidParticle);
        }
    }

private:
    void handlePlayerParticleEffects(Entity &player, bool touchingSolidParticle) {
        if (player.hasComponent<Acceleration>()) {
            auto &accel = player.getComponent<Acceleration>();

            if (touchingSolidParticle) {
                accel.isGrounded = true;
            }
        }

        // if (touchingWater &&player
        // .
        // hasComponent<Velocity>()
        // )
        // {
        //     auto &vel = player.getComponent<Velocity>();
        //     vel.magnitude *= 0.85f;
        // }
        //
        // if (touchingSmoke) {
        //     // placeholder for future smoke effect
        // }
    }
};
#endif //INC_8051TUTORIAL_PARTICLEINTERACTIONSYSTEM_H
