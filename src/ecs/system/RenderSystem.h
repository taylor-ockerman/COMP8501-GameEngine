//
// Created by taylo on 1/21/2026.
//

#ifndef INC_8051TUTORIAL_RENDERSYSTEM_H
#define INC_8051TUTORIAL_RENDERSYSTEM_H
#include <memory>
#include <vector>

#include "../Entity.h"
#include "../Component.h"
#include "../../TextureManager.h"

class RenderSystem {
public:
    void render(const std::vector<std::unique_ptr<Entity> > &entities) {
        Entity *cameraEntity = nullptr;

        //Find camera
        for (auto &e: entities) {
            if (e->hasComponent<Camera>()) {
                cameraEntity = e.get();
                break;
            }
        }

        if (!cameraEntity) return; //no camera
        auto &cam = cameraEntity->getComponent<Camera>();
        for (auto &entity: entities) {
            if (entity->hasComponent<Transform>() && entity->hasComponent<Sprite>()) {
                auto &t = entity->getComponent<Transform>();
                auto &sprite = entity->getComponent<Sprite>();

                if (sprite.renderLayer != RenderLayer::World) continue;
                //we are converting from world space to screen space
                sprite.dst.x = t.position.x - cam.view.x;
                sprite.dst.y = t.position.y - cam.view.y;

                //if the entity has animation component, update the src rect
                if (entity->hasComponent<Animation>()) {
                    auto &anim = entity->getComponent<Animation>();
                    // std::cout << "W: " << anim.clips[anim.currentClip].frameIndices[anim.currentFrame].w << " H: " <<
                    //         anim.clips[anim.
                    //             currentClip].frameIndices[anim.currentFrame].h << " x: " << anim.clips[anim.currentClip]
                    //         .frameIndices[anim
                    //             .currentFrame].x << anim.clips[anim.currentClip].frameIndices[anim.currentFrame].y <<
                    //         " y: " << std::endl;
                    sprite.src = anim.clips[anim.currentClip].frameIndices[anim.currentFrame];
                }

                TextureManager::draw(sprite.texture, &sprite.src, &sprite.dst);
            }
        }
    }
};

#endif //INC_8051TUTORIAL_RENDERSYSTEM_H
