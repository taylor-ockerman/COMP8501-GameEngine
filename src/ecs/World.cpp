//
// Created by taylo on 2/4/2026.
//

#include "World.h"
#include "../Game.h"

World::World() {
    //subscribe to the collision events
    //eventManager.subscribe<CollisionEvent>(onCollisionEvent);
    eventManager.subscribe([this](const CollisionEvent &collision) {
        Entity *sceneStateEntity = nullptr;

        //find scene state
        for (auto &e: entities) {
            if (e->hasComponent<SceneState>()) {
                sceneStateEntity = e.get();
                //std::cout << "Scene State found" << std::endl;
                break;
            }
        }
        if (!sceneStateEntity) return;
        if (collision.entityA == nullptr || collision.entityB == nullptr) {
            return;
        }
        if (!(collision.entityA->hasComponent<Collider>() && collision.entityB->hasComponent<Collider>())) {
            return;
        }
        auto &colliderA = collision.entityA->getComponent<Collider>();
        auto &colliderB = collision.entityB->getComponent<Collider>();

        Entity *player = nullptr;
        Entity *item = nullptr;
        Entity *wall = nullptr;
        Entity *projectile = nullptr;

        if (colliderA.tag == "player" && colliderB.tag == "item") {
            player = collision.entityA;
            item = collision.entityB;
        } else if (colliderA.tag == "item" && colliderB.tag == "player") {
            player = collision.entityB;
            item = collision.entityA;
        }

        if (player && item) {
            item->destroy();
            //scene state
            auto &sceneState = sceneStateEntity->getComponent<SceneState>();
            sceneState.coinsCollected++;
            std::cout << "coins collected:" << sceneState.coinsCollected << std::endl;
            if (sceneState.coinsCollected > 1) {
                std::cout << "Should change scenes now..." << std::endl;
                Game::onSceneChangeRequest("level2");
            }
        }

        //player vs wall
        if (colliderA.tag == "player" && colliderB.tag == "wall") {
            player = collision.entityA;
            wall = collision.entityB;
        } else if (colliderA.tag == "wall" && colliderB.tag == "player") {
            player = collision.entityB;
            wall = collision.entityA;
        }

        if (player && wall) {
            //stop the player
            auto &t = player->getComponent<Transform>();
            t.position = t.oldPosition;
        }

        //player vs projectile
        if (colliderA.tag == "player" && colliderB.tag == "projectile") {
            player = collision.entityA;
            projectile = collision.entityB;
        } else if (colliderA.tag == "projectile" && colliderB.tag == "player") {
            player = collision.entityB;
            projectile = collision.entityA;
        }
        if (player && projectile) {
            player->destroy();
            //change scenes defer
            Game::onSceneChangeRequest("gameover");
        }
    });
}

void onCollisionEvent(const CollisionEvent &collision) {
    std::cout << "A collision occurred between entity A and entity B" << std::endl;
}

