//
// Created by taylo on 1/21/2026.
//

#ifndef INC_8051TUTORIAL_WORLD_H
#define INC_8051TUTORIAL_WORLD_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Map.h"
#include "Entity.h"
#include "GravitySystem.h"
#include "ColliderSyncSystem.h"
#include "ParticleGrid.h"
#include "EventManager.h"
#include "AnimationSystem.h"
#include "KeyboardInputSystem.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "CollisionSystem.h"
#include "CameraSystem.h"
#include "DestructionSystem.h"
#include "SpawnTimerSystem.h"
#include "EventResponseSystem.h"
#include "SceneType.h"
#include "MainMenuSystem.h"
#include "MouseInputSystem.h"
#include "UIRenderSystem.h"

class World {
    Map map;
    std::vector<std::unique_ptr<Entity> > entities;
    std::vector<std::unique_ptr<Entity> > deferredEntities;
    MovementSystem movementSystem;
    RenderSystem renderSystem;
    KeyboardInputSystem keyboardInputSystem;
    CollisionSystem collisionSystem;
    AnimationSystem animationSystem;
    CameraSystem cameraSystem;
    EventManager eventManager;
    SpawnTimerSystem spawnTimerSystem;
    DestructionSystem destructionSystem;
    EventResponseSystem eventResponseSystem{*this};
    MainMenuSystem mainMenuSystem;
    UIRenderSystem uiRenderSystem;
    MouseInputSystem mouseInputSystem;
    GravitySystem gravitySystem;
    ColliderSyncSystem colliderSyncSystem;

public:
    World() = default;

    void update(float dt, const SDL_Event &event, SceneType sceneType, ParticleGrid *grid) {
        if (sceneType == SceneType::MainMenu) {
            mainMenuSystem.update(event);
        } else {
            keyboardInputSystem.update(*this, entities, event);
            gravitySystem.update(entities, gravity, dt);
            movementSystem.update(entities, dt);

            grid->update();
            syncParticlesFromGrid(*grid);

            colliderSyncSystem.update(*this);
            collisionSystem.update(*this);
            animationSystem.update(entities, dt);
            cameraSystem.update(entities);
            spawnTimerSystem.update(entities, dt);
            destructionSystem.update(entities);
        }

        mouseInputSystem.update(*this, event, grid);
        synchronizeEntities();
        cleanup();
    }

    void render(SDL_Renderer *renderer) {
        for (auto &entity: entities) {
            if (entity->hasComponent<Camera>()) {
                map.draw(entity->getComponent<Camera>());
                break;
            }
        }
        renderSystem.render(entities);
        uiRenderSystem.render(entities);
    }

    Entity &createEntity() {
        //use emplace instead of push so we dont create a copy
        entities.emplace_back(std::make_unique<Entity>());
        return *entities.back();
    }

    bool spawnParticleAtWorld(int worldX, int worldY, ParticleGrid& grid, ParticleType type) {
        int cellSize = grid.getCellSize();

        int gx = worldX / cellSize;
        int gy = worldY / cellSize;

        if (!grid.inBounds(gx,gy) || !grid.isEmpty(gx,gy)) return false;

        float x = static_cast<float>(gx * cellSize);
        float y = static_cast<float>(gy * cellSize);

        auto& e = createDeferredEntity();

        e.addComponent<Transform>(Vector2D(x,y), 0.0f, 1.0f, Vector2D(x,y));
        e.addComponent<Particle>(type,gy,gy);
        e.addComponent<Collider>("particle", SDL_FRect{x,y,(float)cellSize,(float)cellSize}, true, Vector2D(0,0));

        SDL_Texture *tex = TextureManager::load("../assets/tileset2.png");
        SDL_FRect src{0,0,64,64};
        switch (type) {
            case ParticleType::Sand:
                src.x = 0;
                src.y = 0;
                //src.w = 64;
                //src.h = 64;
                break;
            case ParticleType::Water:
                src.x = 64;
                src.y = 0;
                //src.w = 64;
                //src.h = 64;
                break;
            case ParticleType::Stone:
                src.x = 192;
                src.y = 192;
                //src.w = 64;
                //src.h = 64;
                break;
            default:
                break;
        }
        //SDL_FRect src{0, 0, 64, 64};
        SDL_FRect dest{x, y, (float)cellSize, (float)cellSize};
        e.addComponent<Sprite>(tex,src,dest,RenderLayer::World, true);

        ParticleType particleType = ParticleType::Empty;
        switch (type) {
            case ParticleType::Sand:
                particleType = ParticleType::Sand;
                break;
            case ParticleType::Water:
                particleType = ParticleType::Water;
                break;
            case ParticleType::Stone:
                particleType = ParticleType::Stone;
                break;
            default:
                particleType = ParticleType::Empty;
                break;
        }

        if (!grid.spawnParticleAtCell(gx,gy,particleType,e)) {
            e.destroy();
            return false;
        }

        return true;
    }

    void destroyAllParticles(ParticleGrid* grid) {
        if (grid != nullptr) {
            grid->clearGrid();
        }

        for (auto& entity : entities) {
            if (entity->hasComponent<Particle>()) {
                entity->destroy();
            }
        }

        for (auto& entity : deferredEntities) {
            if (entity->hasComponent<Particle>()) {
                entity->destroy();
            }
        }

        cleanup();
    }

    void syncParticlesFromGrid(ParticleGrid& grid) {
        int cellSize = grid.getCellSize();
        for (int y = 0; y < grid.getHeight(); y++) {
            for (int x = 0; x < grid.getWidth(); x++) {
                Cell& cell = grid.at(x,y);

                if (cell.type == ParticleType::Empty || cell.entity == nullptr)continue;
                if (!cell.entity->hasComponent<Particle>() || !cell.entity->hasComponent<Transform>()) continue;

                auto& particle = cell.entity->getComponent<Particle>();
                auto& transform = cell.entity->getComponent<Transform>();

                particle.gridX = x;
                particle.gridY = y;

                transform.oldPosition = transform.position;
                transform.position.x = static_cast<float>(x * cellSize);
                transform.position.y = static_cast<float>(y * cellSize);

                if (cell.entity->hasComponent<Collider>()) {
                    auto& collider = cell.entity->getComponent<Collider>();
                    collider.rect.x = transform.position.x + collider.offset.x;
                    collider.rect.y = transform.position.y + collider.offset.y;
                    collider.rect.w = (float)cellSize;
                    collider.rect.h = (float)cellSize;
                }

                if (cell.entity->hasComponent<Sprite>()) {
                    auto& sprite = cell.entity->getComponent<Sprite>();
                    sprite.dst.x = transform.position.x;
                    sprite.dst.y = transform.position.y;
                    sprite.dst.w = (float)sprite.dst.w;
                    sprite.dst.h = (float)sprite.dst.h;
                }
            }
        }
    }

    Entity &createDeferredEntity() {
        deferredEntities.emplace_back(std::make_unique<Entity>());
        return *deferredEntities.back();
    }

    std::vector<std::unique_ptr<Entity> > &getEntities() {
        return entities;
    }

    void cleanup() {
        //use a lambda predicate to remove all inactive entities
        std::erase_if(entities, [](std::unique_ptr<Entity> &e) {
            return !e->isActive();
        });
    }

    void synchronizeEntities() {
        if (!deferredEntities.empty()) {
            //push back all deferred entities to the entities vector
            //using move so we dont create a copy
            std::move(deferredEntities.begin(), deferredEntities.end(), std::back_inserter(entities));
            //clear creation buffer
            deferredEntities.clear();
        }
    }

    void clearAllEntities() {
        entities.clear();
        deferredEntities.clear();
    }

    EventManager &getEventManager() { return eventManager; }

    Map &getMap() { return map; }
    void setSelectedParticle(ParticleType particleType) {
        selectedParticleType = particleType;
    }
    ParticleType getSelectedParticle() const { return selectedParticleType; };
private:
    float gravity = 9.8f;
    ParticleType selectedParticleType = ParticleType::Sand;
};

void onCollisionEvent(const CollisionEvent &collision);
#endif //INC_8051TUTORIAL_WORLD_H
