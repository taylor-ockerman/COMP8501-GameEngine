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
                SDL_FRect renderDst{};
                renderDst.x = (t.position.x - cam.view.x) * cam.zoom;
                renderDst.y = (t.position.y - cam.view.y) * cam.zoom;
                renderDst.w = sprite.dst.w * t.scale * cam.zoom;
                renderDst.h = sprite.dst.h * t.scale * cam.zoom;
                //if the entity has animation component, update the src rect
                if (entity->hasComponent<Animation>()) {
                    auto &anim = entity->getComponent<Animation>();
                    sprite.src = anim.clips[anim.currentClip].frameIndices[anim.currentFrame];
                }
                if (entity->hasComponent<Particle>()) {
                    auto &particle = entity->getComponent<Particle>();
                    if (particle.type == ParticleType::Fire) {
                        if (particle.life % 10 == 0) {
                            sprite.src = ParticleHelpers::getProperties(particle.type, true).spriteSrc;
                        }
                    }
                }
                TextureManager::draw(sprite.texture, &sprite.src, &renderDst);
            }
        }
    }
};

#endif //INC_8051TUTORIAL_RENDERSYSTEM_H
