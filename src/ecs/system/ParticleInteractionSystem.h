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

class ParticleInteractionSystem {
public:
    void update(std::vector<std::unique_ptr<Entity> > &entities, ParticleGrid &grid) {
        for (auto &e: entities) {
            if (!e->hasComponent<Particle>()) continue;

            auto &particle = e->getComponent<Particle>();

            switch (particle.type) {
                case ParticleType::Fire:
                    handleFire(e.get(), grid);
                    break;
                case ParticleType::Smoke:
                    handleSmoke(e.get(), grid);
                    break;
                case ParticleType::Steam:
                    handleSteam(e.get(), grid);
                    break;
                default:
                    break;
            }
        }
    }

private:
    void handleFire(Entity *entity, ParticleGrid &grid) {
        auto &particle = entity->getComponent<Particle>();
        particle.life--;
        if (particle.life <= 0) {
            // 50% chance to become smoke when dying
            if (rand() % 100 < 50) {
                Cell &cell = grid.at(particle.gridX, particle.gridY);
                convertParticle(entity, cell, ParticleType::Smoke, particle.gridX, particle.gridY);
                grid.wakeChunkAndNeighborsForCell(particle.gridX, particle.gridY);
                grid.getChunkFromCell(particle.gridX, particle.gridY).movedThisFrame = true;
            } else {
                destroyParticle(entity, grid);
            }
            return;
        }

        for (auto &[nx, ny]: grid.getNeighbours(particle.gridX, particle.gridY)) {
            if (!grid.inBounds(nx, ny)) continue;

            Cell &neighborCell = grid.at(nx, ny);

            // water extinguishes fire and creates steam
            if (neighborCell.type == ParticleType::Water) {
                Cell &fireCell = grid.at(particle.gridX, particle.gridY);
                convertParticle(entity, fireCell, ParticleType::Steam, particle.gridX, particle.gridY);
                //destory water when it hits fire
                grid.clearCell(nx, ny);
                grid.wakeChunkAndNeighborsForCell(particle.gridX, particle.gridY);
                grid.getChunkFromCell(particle.gridX, particle.gridY).movedThisFrame = true;
                return;
            }

            ParticleType oldType = neighborCell.type;
            ParticleProperties oldProps = ParticleHelpers::getProperties(oldType, false);

            if (!oldProps.flammable) continue;
            if (neighborCell.entity == nullptr) continue;
            if (!neighborCell.entity->hasComponent<Particle>()) continue;

            // chance to ignite
            if (rand() % 100 >= oldProps.ignitionChance) continue;

            convertParticle(neighborCell.entity, neighborCell, ParticleType::Fire, nx, ny);

            // preserve the fuel life from the old material
            auto &neighborParticle = neighborCell.entity->getComponent<Particle>();
            neighborParticle.life = oldProps.life;

            // wood/oil becoming fire should not move like liquid/gas
            neighborCell.behaviour = ParticleBehaviour::Static;

            grid.wakeChunkAndNeighborsForCell(nx, ny);
            grid.getChunkFromCell(nx, ny).movedThisFrame = true;
        }
    }

    void handleSmoke(Entity *entity, ParticleGrid &grid) {
        tickParticleLife(entity, grid, ParticleType::Smoke);
    }

    void handleSteam(Entity *entity, ParticleGrid &grid) {
        tickParticleLife(entity, grid, ParticleType::Steam);
    }

    void tickParticleLife(Entity *entity, ParticleGrid &grid, ParticleType expectedType) {
        if (!entity->hasComponent<Particle>()) return;

        auto &particle = entity->getComponent<Particle>();
        if (particle.type != expectedType) return;

        particle.life--;
        if (particle.life <= 0) {
            destroyParticle(entity, grid);
        }
    }

    void destroyParticle(Entity *entity, ParticleGrid &grid) {
        auto &particle = entity->getComponent<Particle>();

        if (grid.inBounds(particle.gridX, particle.gridY)) {
            grid.wakeChunkAndNeighborsForCell(particle.gridX, particle.gridY);
            grid.getChunkFromCell(particle.gridX, particle.gridY).movedThisFrame = true;
            grid.clearCell(particle.gridX, particle.gridY);
        }

        entity->destroy();
    }

    void convertParticle(Entity *entity, Cell &cell, ParticleType newType, int gx, int gy) {
        auto &particle = entity->getComponent<Particle>();
        ParticleProperties props = ParticleHelpers::getProperties(newType, true);

        particle.type = newType;
        particle.life = props.life;
        particle.gridX = gx;
        particle.gridY = gy;

        cell.type = newType;
        cell.behaviour = props.behaviour;
        cell.entity = entity;

        if (entity->hasComponent<Sprite>()) {
            auto &sprite = entity->getComponent<Sprite>();
            sprite.src = props.spriteSrc;
        }
    }
};

#endif //INC_8051TUTORIAL_PARTICLEINTERACTIONSYSTEM_H
