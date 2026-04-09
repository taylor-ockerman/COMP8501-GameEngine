//
// Created by taylo on 2/18/2026.
//

#ifndef INC_8051TUTORIAL_CAMERASYSTEM_H
#define INC_8051TUTORIAL_CAMERASYSTEM_H
#include <memory>
#include <vector>
#include "Entity.h"
#include "Component.h"

class CameraSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity> > &entities) {
        Entity *playerEntity = nullptr;
        for (auto &e: entities) {
            if (e->hasComponent<PlayerTag>()) {
                playerEntity = e.get();
                break;
            }
        }
        if (!playerEntity) return;

        auto &playerTransform = playerEntity->getComponent<Transform>();

        for (auto &e: entities) {
            if (e->hasComponent<Camera>()) {
                auto &cam = e->getComponent<Camera>();
                //accounts for camera zoom level now
                float visibleWorldWidth = cam.view.w / cam.zoom;
                float visibleWorldHeight = cam.view.h / cam.zoom;

                float targetX = playerTransform.position.x - visibleWorldWidth * 0.5f;
                float targetY = playerTransform.position.y - visibleWorldHeight * 0.5f;

                float maxX = std::max(0.0f, cam.worldWidth - visibleWorldWidth);
                float maxY = std::max(0.0f, cam.worldHeight - visibleWorldHeight);

                cam.view.x = std::clamp(targetX, 0.0f, maxX);
                cam.view.y = std::clamp(targetY, 0.0f, maxY);
            }
        }
    }
};

#endif //INC_8051TUTORIAL_CAMERASYSTEM_H
