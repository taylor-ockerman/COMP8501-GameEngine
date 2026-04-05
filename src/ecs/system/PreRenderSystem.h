//
// Created by taylo on 4/1/2026.
//

#ifndef INC_8051TUTORIAL_PRERENDERSYSTEM_H
#define INC_8051TUTORIAL_PRERENDERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "../../manager/TextureManager.h"

class PreRenderSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity> > &entities) {
        for (auto &entity: entities) {
            if (entity->hasComponent<Label>() && entity->getComponent<Label>().dirty) {
                //update label texture
                TextureManager::updateLabel(entity->getComponent<Label>());
            }
        }
    }
};
#endif //INC_8051TUTORIAL_PRERENDERSYSTEM_H
