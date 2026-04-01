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
#include "ParticleSimulationSystem.h"
#include "UIRenderSystem.h"
#include "ParticlePlacementHelper.h"
#include "ParticleSyncSystem.h"
#include "ParticleInteractionSystem.h"

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
    ParticleSimulationSystem particleSimulationSystem;
    ParticleSyncSystem particleSyncSystem;
    ParticleInteractionSystem particleInteractionSystem;

public:
    World() = default;

    void update(float dt, const SDL_Event &event, SceneType sceneType, ParticleGrid *grid) {
        if (sceneType == SceneType::MainMenu) {
            mainMenuSystem.update(event);
        } else {
            keyboardInputSystem.update(*this, entities, event);
            gravitySystem.update(entities, gravity, dt);
            movementSystem.update(entities, dt);
            colliderSyncSystem.update(*this);
            particleSimulationSystem.update(*grid, entities);
            particleSyncSystem.update(*grid);
            //particleInteractionSystem.update(entities, *grid);

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
        //use emplace instead of push so we don't create a copy
        entities.emplace_back(std::make_unique<Entity>());
        return *entities.back();
    }

    bool spawnParticleAtWorld(int worldX, int worldY, ParticleGrid &grid, ParticleType type) {
        //checks if spawning is valid
        int cellSize = grid.getCellSize();

        int gx = worldX / cellSize;
        int gy = worldY / cellSize;
        if ((!grid.inBounds(gx, gy) || !grid.isEmpty(gx, gy))) return false;
        if (grid.at(gx, gy).occupiedByPlayer) return false;
        //TODO change is empty check so particles can overwrite each other under certain conditions
        return spawnParticleAtCell(gx, gy, grid, type);
    }

    void spawnBrushAtWorld(int worldX, int worldY, ParticleGrid &grid, ParticleType type) {
        int cellSize = grid.getCellSize();
        for (int dy = -brushSize; dy <= brushSize; dy++) {
            for (int dx = -brushSize; dx <= brushSize; dx++) {
                if (dx * dx + dy * dy > brushSize * brushSize) continue;
                spawnParticleAtWorld(worldX + dx * cellSize, worldY + dy * cellSize, grid, type);
            }
        }
    }

    bool spawnParticleAtCell(int gx, int gy, ParticleGrid &grid, ParticleType type) {
        int cellSize = grid.getCellSize();
        auto &e = createDeferredEntity();
        float worldX = static_cast<float>(gx * cellSize);
        float worldY = static_cast<float>(gy * cellSize);
        e.addComponent<Transform>(Vector2D(worldX, worldY), 0.0f, 1.0f, Vector2D(gx, gy));
        e.addComponent<Particle>(type, gx, gy);
        // colliders are a huge performance hit, disabling for now
        // e.addComponent<Collider>("particle", SDL_FRect{worldX, worldY, (float) cellSize, (float) cellSize},
        //                          true,
        //                          Vector2D(0, 0));

        SDL_Texture *tex = TextureManager::load("../assets/tileset2.png");
        SDL_FRect src{0, 0, 64, 64};
        switch (type) {
            case ParticleType::Sand:
                src.x = 0;
                src.y = 0;
                //src.w = 64;
                //src.h = 64;
                break;
            case ParticleType::Water:
                src.x = 0;
                src.y = 128;
                //src.w = 64;
                //src.h = 64;
                break;
            case ParticleType::Stone:
                src.x = 192;
                src.y = 192;
                //src.w = 64;
                //src.h = 64;
                break;
            case ParticleType::Smoke:
                src.x = 192;
                src.y = 128;
            default:
                break;
        }
        //SDL_FRect src{0, 0, 64, 64};
        SDL_FRect dest{worldX, worldY, (float) cellSize, (float) cellSize};
        e.addComponent<Sprite>(tex, src, dest, RenderLayer::World, true);

        ParticleType particleType = ParticleType::Empty;
        switch (type) {
            case ParticleType::Sand:
                particleType = ParticleType::Sand;
                e.addComponent<ParticleProperties>(ParticleHelpers::getProperties(ParticleType::Sand));
                break;
            case ParticleType::Water:
                particleType = ParticleType::Water;
                e.addComponent<ParticleProperties>(ParticleHelpers::getProperties(ParticleType::Water));
                break;
            case ParticleType::Stone:
                particleType = ParticleType::Stone;
                e.addComponent<ParticleProperties>(ParticleHelpers::getProperties(ParticleType::Stone));
                break;
            case ParticleType::Smoke:
                particleType = ParticleType::Smoke;
                e.addComponent<ParticleProperties>(ParticleHelpers::getProperties(ParticleType::Smoke));
                break;
            default:
                particleType = ParticleType::Empty;
                break;
        }

        if (!ParticlePlacementHelper::spawnParticleAtCell(grid, gx, gy, particleType, e)) {
            e.destroy();
            return false;
        }

        return true;
    }

    void eraseBrushAtWorld(int worldX, int worldY, ParticleGrid &grid) {
        int cellSize = grid.getCellSize();

        int centerGX = worldX / cellSize;
        int centerGY = worldY / cellSize;

        for (int dy = -brushSize; dy <= brushSize; dy++) {
            for (int dx = -brushSize; dx <= brushSize; dx++) {
                if (dx * dx + dy * dy > brushSize * brushSize) continue;

                int gx = centerGX + dx;
                int gy = centerGY + dy;

                if (!grid.inBounds(gx, gy)) continue;

                Cell &cell = grid.at(gx, gy);

                // usually you do not want to erase walls
                if (cell.type == ParticleType::Wall) continue;

                grid.clearCell(gx, gy);
                grid.wakeChunkAndNeighborsForCell(gx, gy);
                grid.getChunkFromCell(gx, gy).movedThisFrame = true;
            }
        }
    }

    void destroyAllParticles(ParticleGrid *grid) {
        if (grid != nullptr) {
            grid->clearGrid();
        }

        for (auto &entity: entities) {
            if (entity->hasComponent<Particle>()) {
                entity->destroy();
            }
        }

        for (auto &entity: deferredEntities) {
            if (entity->hasComponent<Particle>()) {
                entity->destroy();
            }
        }

        cleanup();
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
            //using move so we don't create a copy
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

    void setSelectedBrushTool(BrushTool brushTool) {
        selectedBrushTool = brushTool;
    }

    //ParticleType getSelectedParticle() const { return selectedParticleType; };
    BrushTool getSelectedBrushTool() const { return selectedBrushTool; }
    int getBrushSize() { return brushSize; }
    int getMaxBrushSize() { return maxBrushSize; }

    void incrementBrushSize() {
        brushSize++;
        if (brushSize > maxBrushSize) {
            brushSize = maxBrushSize;
        }
    }

    void decrementBrushSize() {
        brushSize--;
        if (brushSize <= 0) {
            brushSize = 0;
        }
    }

private:
    float gravity = 9.8f;
    //ParticleType selectedParticleType = ParticleType::Sand;
    BrushTool selectedBrushTool = BrushTool::Sand;
    int brushSize = 1;
    int maxBrushSize = 8;
};

void onCollisionEvent(const CollisionEvent &collision);
#endif //INC_8051TUTORIAL_WORLD_H
