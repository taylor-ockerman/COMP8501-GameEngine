//
// Created by taylo on 4/4/2026.
//
#include "HUDSystem.h"
#include "World.h"

void HUDSystem::update(const std::vector<std::unique_ptr<Entity> > &entities) {
    Entity *playerEntity = nullptr;
    Entity *spawnerHUD = nullptr;
    Entity *camera = nullptr;
    //find player
    for (auto &entity: entities) {
        if (entity->hasComponent<PlayerTag>()) {
            playerEntity = entity.get();
        }
        if (entity->hasComponent<SpawnerHUDTag>()) {
            spawnerHUD = entity.get();
        }
        if (spawnerHUD && playerEntity) break;
    }

    if (spawnerHUD && spawnerHUD->hasComponent<Sprite>()) {
    }
    if (!playerEntity) return;
    auto &playerTransform = playerEntity->getComponent<Transform>();

    for (auto &e: entities) {
        if (e->hasComponent<Label>()) {
            auto &label = e->getComponent<Label>();

            if (label.type == LabelType::PlayerPosition) {
                std::stringstream ss;
                ss << "Player position: " << playerTransform.position;
                label.text = ss.str();
                label.dirty = true;
            }
        }
    }
}


