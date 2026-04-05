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
#include "AudioEventQueue.h"
#include "KeyboardInputSystem.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "CollisionSystem.h"
#include "CameraSystem.h"
#include "DestructionSystem.h"
#include "SpawnTimerSystem.h"
#include "EventResponseSystem.h"
#include "HUDSystem.h"
#include "SceneType.h"
#include "MainMenuSystem.h"
#include "MouseInputSystem.h"
#include "ParticleSimulationSystem.h"
#include "UIRenderSystem.h"
#include "ParticlePlacementHelper.h"
#include "ParticleSyncSystem.h"
#include "ParticleInteractionSystem.h"
#include "PreRenderSystem.h"
#include "ParticleSpawnDeSpawnSystem.h"

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
    ParticleSpawnDeSpawnSystem particleCreationSystem;
    HUDSystem hudSystem;
    PreRenderSystem preRenderSystem;
    AudioEventQueue audioEventQueue;

public:
    World() = default;

    void update(float dt, const SDL_Event &event, SceneType sceneType, ParticleGrid *grid) {
        if (sceneType == SceneType::MainMenu) {
            mainMenuSystem.update(event);
        } else {
            keyboardInputSystem.update(*this, entities, event);
            gravitySystem.update(entities, dt);
            movementSystem.update(entities, dt);
            colliderSyncSystem.update(*this);
            particleSimulationSystem.update(*grid, entities);
            particleSyncSystem.update(*grid);
            particleInteractionSystem.update(entities, *grid);
            particleCreationSystem.update(*this, *grid);
            collisionSystem.update(*this);
            animationSystem.update(entities, dt);
            cameraSystem.update(entities);
            spawnTimerSystem.update(entities, dt);
            destructionSystem.update(entities);
            hudSystem.update(entities);
        }

        mouseInputSystem.update(*this, event, grid);
        // if (sceneType != SceneType::MainMenu && grid != nullptr) {
        //     //a bit janky but this needs to come after mouseinput
        //     //system since it is responsible for actually creating and deleting from mouse clicks
        //     //particleCreationSystem.update(*this, *grid);
        // }
        audioEventQueue.process();
        preRenderSystem.update(entities);
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
        uiRenderSystem.render(*this, entities);
    }

    Entity &createEntity() {
        //use emplace instead of push so we don't create a copy
        entities.emplace_back(std::make_unique<Entity>());
        return *entities.back();
    }

    void printParticleCounts(ParticleGrid &grid) {
        int particleCount = 0;

        for (auto &e: entities) {
            if (e->hasComponent<Particle>()) particleCount++;
        }
        std::cout << "entities=" << entities.size()
                << " particles=" << particleCount
                << " activeChunks=" << grid.getActiveChunkCount()
                << " nonEmptyCells=" << grid.countNonEmptyCells()
                << std::endl;
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

    std::vector<std::unique_ptr<Entity> > &getDeferredEntities() { return deferredEntities; }

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

    AudioEventQueue &getAudioEventQueue() { return audioEventQueue; }
    EventManager &getEventManager() { return eventManager; }
    Map &getMap() { return map; }
};

void onCollisionEvent(const CollisionEvent &collision);
#endif //INC_8051TUTORIAL_WORLD_H
