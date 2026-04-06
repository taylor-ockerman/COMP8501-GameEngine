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
    });

    world.getEventManager().subscribe([this, &world](const BaseEvent &e) {
        if (e.type != EventType::MenuToggle) return;
        const auto &menuToggleEvent = static_cast<const MenuToggleEvent &>(e);
        onMenuToggle(menuToggleEvent, world);
    });

    world.getEventManager().subscribe([this,&world](const BaseEvent &e) {
        if (e.type != EventType::FireExtinguish) return;
        const auto &fireExtinguishEvent = static_cast<const FireExtinguishEvent &>(e);
        onFireExtinguish(fireExtinguishEvent, world);
    });
    world.getEventManager().subscribe([this,&world](const BaseEvent &e) {
        if (e.type != EventType::SpawnAudio) return;
        const auto &spawnAudioEvent = static_cast<const SpawnAudioEvent &>(e);
        onSpawnAudio(spawnAudioEvent, world);
    });
}

void EventResponseSystem::onCollision(const CollisionEvent &e, const char *otherTag, World &world) {
    Entity *player = nullptr;
    Entity *other = nullptr;
    if (!getCollisionEntities(e, otherTag, player, other)) return;
    if (std::string(otherTag) == "item") {
        if (e.state != CollisionState::Enter) return;
        world.getAudioEventQueue().push(std::make_unique<AudioEvent>("coinPickUp", AudioCommand::PlayOneShot));
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
    BrushState *state = nullptr;
    for (auto &ent: world.getEntities()) {
        if (ent->hasComponent<SpawnerHUDTag>()) {
            spawner = ent.get();
        }
        if (ent->hasComponent<BrushState>()) {
            state = &ent->getComponent<BrushState>();
        }
    }
    if (!spawner) {
        std::cout << "no spawner hud element found" << std::endl;
        return;
    }
    auto &sprite = spawner->getComponent<Sprite>();
    if (!state) {
        std::cout << "no brushState" << std::endl;
        return;
    }
    if (spawner->hasComponent<Children>()) {
        for (auto &ent: spawner->getComponent<Children>().children) {
            if (ent->hasComponent<Label>()) {
                auto &label = ent->getComponent<Label>();
                label.text = ParticleHelpers::particleTypeToString(e.pType);
                label.dirty = true;
            }
        }
    }
    state->selectedParticle = e.pType;
    SDL_Texture *tex = TextureManager::load("../assets/particle_tileset.png");
    SDL_FRect src = ParticleHelpers::getProperties(e.pType, false).spriteSrc;
    //std::cout << "hitting that shiz" << std::endl;
    src.w += 64;
    src.h += 64;
    SDL_FRect dst = sprite.dst;
    spawner->deactivateComponent<Sprite>();
    spawner->addComponent<Sprite>(tex, src, dst, RenderLayer::UI, true);
    //sprite = Sprite{tex, src, dst, RenderLayer::UI, true};
}

void EventResponseSystem::onMenuToggle(const MenuToggleEvent &e, World &world) {
    Entity *menu = nullptr;
    for (auto &e: world.getEntities()) {
        if (e->hasComponent<MenuTag>() && e->hasComponent<Sprite>()) {
            menu = e.get();
            break;
        }
    }
    if (!menu) {
        std::cout << "no menu found" << std::endl;
        return;
    }

    auto &sprite = menu->getComponent<Sprite>();
    bool newVisibility = !sprite.visible;
    sprite.visible = newVisibility;

    if (menu->hasComponent<Children>()) {
        auto &children = menu->getComponent<Children>();
        for (auto &child: children.children) {
            if (child->hasComponent<Sprite>()) {
                child->getComponent<Sprite>().visible = newVisibility;
            }
            if (child->hasComponent<Label>()) {
                child->getComponent<Label>().visible = newVisibility;
            }
        }
    }
};

void EventResponseSystem::onFireExtinguish(const FireExtinguishEvent &e, World &world) {
    world.getAudioEventQueue().push(std::make_unique<AudioEvent>("waterHitsFire", AudioCommand::PlayOneShot));
}

void EventResponseSystem::onSpawnAudio(const SpawnAudioEvent &e, World &world) {
    std::string clip = ParticleHelpers::spawnClipForParticle(e.pType);
    if (clip.empty()) return;

    if (e.start) {
        world.getAudioEventQueue().push(
            std::make_unique<AudioEvent>(clip, AudioCommand::StartSpawnLoop)
        );
    } else {
        world.getAudioEventQueue().push(
            std::make_unique<AudioEvent>("", AudioCommand::StopSpawnLoop)
        );
    }
}
