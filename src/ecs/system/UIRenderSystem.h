//
// Created by taylo on 3/11/2026.
//

#ifndef INC_8051TUTORIAL_UIRENDERSYSTEM_H
#define INC_8051TUTORIAL_UIRENDERSYSTEM_H
#include <memory>
#include <ranges>

#include "../Entity.h"
#include "../Component.h"
#include "../../utils/RenderUtils.h"
#include "../../TextureManager.h"

class UIRenderSystem {
public:
    void render(const std::vector<std::unique_ptr<Entity> > &entities) {
        for (auto &e: entities) {
            if (e->hasComponent<Transform>()) {
                auto transform = e->getComponent<Transform>();
                if (e->hasComponent<Sprite>()) {
                    auto sprite = e->getComponent<Sprite>();

                    if (sprite.renderLayer != RenderLayer::UI) continue;

                    //no converting from world space to screen space
                    sprite.dst.x = transform.position.x;
                    sprite.dst.y = transform.position.y;

                    if (sprite.visible) {
                        SDL_FRect scaledDest = RenderUtils::getScaledDest(sprite.dst, transform.scale);
                        TextureManager::draw(sprite.texture, &sprite.src, &scaledDest);
                    }
                } else if (e->hasComponent<Label>()) {
                    auto label = e->getComponent<Label>();

                    label.dst.x = transform.position.x;
                    label.dst.y = transform.position.y;

                    if (label.visible) {
                        SDL_FRect scaledDest = RenderUtils::getScaledDest(label.dst, transform.scale);
                        TextureManager::draw(label.texture, nullptr, &scaledDest);
                    }
                }
            }
        }
    }
};

#endif //INC_8051TUTORIAL_UIRENDERSYSTEM_H
