//
// Created by taylo on 2/25/2026.
//

#ifndef INC_8051TUTORIAL_DESTRUCTIONSYSTEM_H
#define INC_8051TUTORIAL_DESTRUCTIONSYSTEM_H
#include <memory>
#include <vector>
#include "../Entity.h"
#include "../Component.h"

class DestructionSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity> > &entities) {
        Entity *cameraEntity = nullptr;

        //find camera
        for (auto &e: entities) {
            if (e->hasComponent<Camera>()) {
                cameraEntity = e.get();
                break;
            }
        }

        if (!cameraEntity) return;
        auto &cam = cameraEntity->getComponent<Camera>();

        for (auto &e: entities) {
            if (e->hasComponent<Transform>() && e->hasComponent<ProjectileTag>()) {
                auto &t = e->getComponent<Transform>();
                //destroy entity if it goes out the cam view
                if (t.position.x > cam.view.x + cam.view.w || //right edge
                    t.position.x < cam.view.x || //left edge
                    t.position.y > cam.view.y + cam.view.h || //bottom
                    t.position.y < cam.view.y) {
                    // top

                    e->destroy();
                }
            }
        }
    }
};
#endif //INC_8051TUTORIAL_DESTRUCTIONSYSTEM_H
