//
// Created by taylo on 3/18/2026.
//

#include "SpriteOffsetSystem.h"
#include "World.h"

void SpriteOffsetSystem::update(World &world) {
    const std::vector<Entity *> entities = queryCollidableSprites(world.getEntities());

    for (auto e: entities) {
        auto &t = e->getComponent<Transform>();
        auto &c = e->getComponent<Collider>();
        //applying offset to player collider so it better matches the sprite
        c.rect.x = t.position.x + c.offset.x;
        c.rect.y = t.position.y + c.offset.y;
    }
}

std::vector<Entity *> SpriteOffsetSystem::
queryCollidableSprites(const std::vector<std::unique_ptr<Entity> > &entities) {
    std::vector<Entity *> collidablesSprites;
    for (auto &entity: entities) {
        if (entity->hasComponent<Transform>() && entity->hasComponent<Collider>() && entity->hasComponent<Sprite>()) {
            collidablesSprites.push_back(entity.get());
        }
    }
    return collidablesSprites;
}

