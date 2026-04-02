//
// Created by taylo on 4/1/2026.
//

#ifndef INC_8051TUTORIAL_HUDSYSTEM_H
#define INC_8051TUTORIAL_HUDSYSTEM_H
#include <memory>
#include <sstream>
#include <vector>

#include "Component.h"
#include "Entity.h"

class HUDSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity> > &entities) {
        Entity *playerEntity = nullptr;
        Entity *spawnerHUD = nullptr;
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
};
#endif //INC_8051TUTORIAL_HUDSYSTEM_H
