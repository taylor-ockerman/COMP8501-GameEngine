//
// Created by taylo on 4/4/2026.
//
#include "UIRenderSystem.h"
#include "World.h"
#include "RenderUtils.h"

void UIRenderSystem::render(World &world, const std::vector<std::unique_ptr<Entity> > &entities) {
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
    drawBrush(world, entities);
}

void UIRenderSystem::drawBrush(World &world, const std::vector<std::unique_ptr<Entity> > &entities) {
    int centerX = world.getMouseScreenX();
    int centerY = world.getMouseScreenY();

    int brushSize = world.getBrushSize();

    const float radiusScale = 4.0f; //should be same value as cell size
    const float baseRadius = 1.0f;

    float radius = brushSize * radiusScale + baseRadius;

    Entity *cam = nullptr;
    for (auto &e: world.getEntities()) {
        if (e->hasComponent<Camera>()) {
            cam = e.get();
        }
    }
    if (cam && cam->hasComponent<Camera>()) {
        Vector2D center{(float) centerX, (float) centerY};
        TextureManager::drawCircle(cam, center, radius, 255, 255, 255);
    }
}
