//
// Created by taylo on 1/21/2026.
//

#ifndef INC_8051TUTORIAL_WORLD_H
#define INC_8051TUTORIAL_WORLD_H
#include <memory>
#include <vector>

#include "Component.h"
#include "../Map.h"
#include "Entity.h"
#include "HUDSystem.h"
#include "PreRenderSystem.h"
#include "event/EventManager.h"
#include "system/AnimationSystem.h"
#include "system/KeyboardInputSystem.h"
#include "system/MovementSystem.h"
#include "system/RenderSystem.h"
#include "system/CollisionSystem.h"
#include "system/KeyboardInputSystem.h"
#include "system/CameraSystem.h"
#include "system/DestructionSystem.h"
#include "system/SpawnTimerSystem.h"
#include "system/EventResponseSystem.h"
#include "../scene/SceneType.h"
#include "system/MainMenuSystem.h"
#include "system/MouseInputSystem.h"
#include "system/UIRenderSystem.h"

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
    HUDSystem hudSystem;
    PreRenderSystem preRenderSystem;
public:
    World() = default;

    void update(float dt, const SDL_Event &event, SceneType sceneType) {
        if (sceneType == SceneType::MainMenu) {
            mainMenuSystem.update(event);
        } else {
            keyboardInputSystem.update(entities, event);
            movementSystem.update(entities, dt);
            collisionSystem.update(*this);
            animationSystem.update(entities, dt);
            cameraSystem.update(entities);
            spawnTimerSystem.update(entities, dt);
            destructionSystem.update(entities);
            hudSystem.update(entities);

        }

        mouseInputSystem.update(*this, event);
        preRenderSystem.update(entities);
        synchronizeEntities();
        cleanup();
    }

    void render() {
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

    EventManager &getEventManager() { return eventManager; }

    Map &getMap() { return map; }
};

void onCollisionEvent(const CollisionEvent &collision);
#endif //INC_8051TUTORIAL_WORLD_H
