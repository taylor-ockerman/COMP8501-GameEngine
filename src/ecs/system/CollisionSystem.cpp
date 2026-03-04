//
// Created by taylo on 2/4/2026.
//
#include "CollisionSystem.h"
#include "../World.h"
#include "../../utils/Collision.h"
//let's say we have three entities eA, eB, eC
//1. positions the collider with the transform
//2. checking for collisions
//
void CollisionSystem::update(World &world) {
    //get a list of entities that have colliders and transforms
    const std::vector<Entity *> collidables = queryCollidables(world.getEntities());

    //update all collider positions first
    for (auto entity: collidables) {
        auto &t = entity->getComponent<Transform>();
        auto &c = entity->getComponent<Collider>();
        c.rect.x = t.position.x;
        c.rect.y = t.position.y;
    }
    //outer loop
    for (size_t i = 0; i < collidables.size(); ++i) {
        auto entityA = collidables[i];
        auto &colliderA = entityA->getComponent<Collider>();

        //check for the collider collision
        //inner loop
        for (size_t j = i + 1; j < collidables.size(); ++j) {
            auto &entityB = collidables[j];
            auto &colliderB = entityB->getComponent<Collider>();

            if (Collision::AABB(colliderA, colliderB)) {
                world.getEventManager().emit(CollisionEvent{entityA, entityB});
            }
        }
    }
}

std::vector<Entity *> CollisionSystem::queryCollidables(const std::vector<std::unique_ptr<Entity> > &entities) {
    std::vector<Entity *> collidables;
    for (auto &entity: entities) {
        if (entity->hasComponent<Transform>() && entity->hasComponent<Collider>()) {
            collidables.push_back(entity.get());
        }
    }
    return collidables;
}
