//
// Created by taylo on 4/6/2026.
//
#include "ParticleInteractionSystem.h"
#include "World.h"

void ParticleInteractionSystem::update(World &world, std::vector<std::unique_ptr<Entity> > &entities,
                                       ParticleGrid &grid) {
    for (auto &e: entities) {
        if (!e->hasComponent<Particle>()) continue;

        auto &particle = e->getComponent<Particle>();

        switch (particle.type) {
            case ParticleType::Fire:
                handleFire(world, e.get(), grid);
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

//most complicated interaction, bit messy of an implementation
void ParticleInteractionSystem::handleFire(World &world, Entity *entity, ParticleGrid &grid) {
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
    //check neighbours for water or flammable materials
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
            world.getEventManager().emit(FireExtinguishEvent{});
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

        // fire behavior is static, so we need to set this after conversion
        neighborCell.behaviour = ParticleBehaviour::Static;

        grid.wakeChunkAndNeighborsForCell(nx, ny);
        grid.getChunkFromCell(nx, ny).movedThisFrame = true;
    }
}


void ParticleInteractionSystem::tickParticleLife(Entity *entity, ParticleGrid &grid, ParticleType expectedType) {
    if (!entity->hasComponent<Particle>()) return;

    auto &particle = entity->getComponent<Particle>();
    if (particle.type != expectedType) return;

    particle.life--;
    if (particle.life <= 0) {
        destroyParticle(entity, grid);
    }
}

void ParticleInteractionSystem::handleSmoke(Entity *entity, ParticleGrid &grid) {
    tickParticleLife(entity, grid, ParticleType::Smoke);
}

void ParticleInteractionSystem::handleSteam(Entity *entity, ParticleGrid &grid) {
    tickParticleLife(entity, grid, ParticleType::Steam);
}

void ParticleInteractionSystem::convertParticle(Entity *entity, Cell &cell, ParticleType newType, int gx, int gy) {
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

void ParticleInteractionSystem::destroyParticle(Entity *entity, ParticleGrid &grid) {
    auto &particle = entity->getComponent<Particle>();

    if (grid.inBounds(particle.gridX, particle.gridY)) {
        grid.wakeChunkAndNeighborsForCell(particle.gridX, particle.gridY);
        grid.getChunkFromCell(particle.gridX, particle.gridY).movedThisFrame = true;
        grid.clearCell(particle.gridX, particle.gridY);
    }

    entity->destroy();
}

