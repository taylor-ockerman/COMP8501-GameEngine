//
// Created by taylo on 3/11/2026.
//
#include "EventResponseSystem.h"

#include <numeric>

#include "Collision.h"
#include "CollisionResolution.h"
#include "World.h"
#include "Game.h"

EventResponseSystem::EventResponseSystem(World &world) {
    //subscriptions
    world.getEventManager().subscribe(
        [this,&world](const BaseEvent &e) {
            if (e.type != EventType::Collision) return;
            const auto &collision = static_cast<const CollisionEvent &>(e); //cast base type to collision type
            onCollision(collision, "item", world);
            onCollision(collision, "wall", world);
            onCollision(collision, "projectile", world);

            //don't need this anymore as colliders are disabled
            //onCollision(collision, "particle", world);
        });

    world.getEventManager().subscribe(
        [this,&world](const BaseEvent &e) {
            if (e.type != EventType::PlayerAction) return;
            const auto &playerAction = static_cast<const PlayerActionEvent &>(e); //cast base type to collision type
        });

    world.getEventManager().subscribe(
        [this,&world](const BaseEvent &e) {
            if (e.type != EventType::MouseInteraction) return;
            const auto &mouseInteractionEvent = static_cast<const MouseInteractionEvent &>(e);
            onMouseInteraction(mouseInteractionEvent);
        });

    world.getEventManager().subscribe([this, &world](const BaseEvent &e) {
        if (e.type != EventType::SpawnerChange) return;
        const auto &spawnerChangeEvent = static_cast<const SpawnerChangeEvent &>(e);
        onSpawnerChangeEvent(spawnerChangeEvent, world);
        std::cout << "subscribed" << std::endl;
    });
}

void EventResponseSystem::onCollision(const CollisionEvent &e, const char *otherTag, World &world) {
    Entity *player = nullptr;
    Entity *other = nullptr;
    if (!getCollisionEntities(e, otherTag, player, other)) return;
    if (std::string(otherTag) == "item") {
        if (e.state != CollisionState::Enter) return;
        world.getAudioEventQueue().push(std::make_unique<AudioEvent>("coinPickUp"));
        other->destroy();
        //scene state
        for (auto &entity: world.getEntities()) {
            if (!entity->hasComponent<SceneState>()) continue;

            auto &sceneState = entity->getComponent<SceneState>();
            sceneState.coinsCollected++;
            std::cout << "coins collected:" << sceneState.coinsCollected << std::endl;
            if (sceneState.coinsCollected > 1) {
                std::cout << "Should change scenes now..." << std::endl;
                Game::onSceneChangeRequest("level2");
            }
        }
    } else if (std::string(otherTag) == "wall") {
        if (e.state == CollisionState::Exit) {
            player->getComponent<Acceleration>().isGrounded = false;
            return;
        }
        if (e.state != CollisionState::Stay) return;
        //stop the player
        //std::cout << "Player wall collision!!";
        CollisionResolution::resolvePlayerWall(*player, *other);
    } else if (std::string(otherTag) == "projectile") {
        if (e.state != CollisionState::Enter) return;

        //this logic is simple and direct
        //ideally we would only operate on data in an update function (hinting at transient entities)
        auto &health = player->getComponent<Health>();
        health.currentHealth--;

        Game::gameState.playerHealth = health.currentHealth;

        std::cout << "health changed:" << health.currentHealth << std::endl;
        if (health.currentHealth <= 0) {
            player->destroy();
            Game::onSceneChangeRequest("gameover");
        }
        //change scenes defer
    } else if (std::string(otherTag) == "particle") {
        //std::cout << "in particle collision" << std::endl;
        if (e.state == CollisionState::Exit) {
            player->getComponent<Acceleration>().isGrounded = false;
            return;
        }
        if (e.state != CollisionState::Stay) return;
        CollisionResolution::resolvePlayerWall(*player, *other);
    }
}

bool EventResponseSystem::getCollisionEntities(const CollisionEvent &e, const char *otherTag, Entity *&player,
                                               Entity *&other) {
    if (e.entityA == nullptr || e.entityB == nullptr) {
        return false;
    }
    if (!(e.entityA->hasComponent<Collider>() && e.entityB->hasComponent<Collider>())) {
        return false;
    }
    auto &colliderA = e.entityA->getComponent<Collider>();
    auto &colliderB = e.entityB->getComponent<Collider>();

    if (colliderA.tag == "player" && colliderB.tag == otherTag) {
        player = e.entityA;
        other = e.entityB;
    } else if (colliderA.tag == otherTag && colliderB.tag == "player") {
        player = e.entityB;
        other = e.entityA;
    }
    return player && other;
}

void EventResponseSystem::onPlayerAction(const CollisionEvent &e,
                                         const std::function<void(Entity *player, PlayerAction action)> &callback) {
}

void EventResponseSystem::onMouseInteraction(const MouseInteractionEvent &e) {
    if (!e.entity->hasComponent<Clickable>()) return;

    auto &clickable = e.entity->getComponent<Clickable>();
    switch (e.state) {
        case MouseInteractionState::Pressed:
            clickable.onPressed();
            break;
        case MouseInteractionState::Released:
            clickable.onReleased();
            break;
        case MouseInteractionState::Cancel:
            clickable.onCancel();
            break;
        default:
            break;
    }
}

void EventResponseSystem::
onSpawnerChangeEvent(const SpawnerChangeEvent &e, World &world) {
    Entity *spawner = nullptr;
    for (auto &e: world.getEntities()) {
        if (e->hasComponent<SpawnerHUDTag>()) {
            spawner = e.get();
        }
    }
    if (!spawner) {
        std::cout << "no spawner hud element found" << std::endl;
        return;
    }
    auto &sprite = spawner->getComponent<Sprite>();
    SDL_Texture *tex = TextureManager::load("../assets/tileset2.png");
    SDL_FRect src{0, 0, 64, 64};
    std::cout << "hitting that shiz" << std::endl;
    switch (e.pType) {
        case ParticleType::Sand:
            src.x = 0;
            src.y = 0;
            break;
        case ParticleType::Water:
            src.x = 0;
            src.y = 128;
            break;
        case ParticleType::Stone:
            src.x = 192;
            src.y = 192;
            break;
        case ParticleType::Smoke:
            src.x = 192;
            src.y = 128;
        case ParticleType::Empty:
            src.x = 0;
            src.y = 160;
            break;
        default:
            break;
    }

    SDL_FRect dst = sprite.dst;
    spawner->deactivateComponent<Sprite>();
    spawner->addComponent<Sprite>(tex, src, dst, RenderLayer::UI, true);
    //sprite = Sprite{tex, src, dst, RenderLayer::UI, true};
}
