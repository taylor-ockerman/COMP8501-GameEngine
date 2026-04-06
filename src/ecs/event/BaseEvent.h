//
// Created by taylo on 3/11/2026.
//

#ifndef INC_8051TUTORIAL_BASEEVENT_H
#define INC_8051TUTORIAL_BASEEVENT_H

#include "Component.h"
#include "Entity.h"

enum class EventType {
    Collision,
    PlayerAction,
    MouseInteraction,
    SpawnerChange,
    MenuToggle
};

struct BaseEvent {
    EventType type{};

    virtual ~BaseEvent() = default; //in c++ always the destructor virtual from a class you are deriving from
};

enum class CollisionState { Enter, Stay, Exit };

struct CollisionEvent : BaseEvent {
    Entity *entityA = nullptr;
    Entity *entityB = nullptr;
    CollisionState state{};

    CollisionEvent(Entity *entityA, Entity *entityB, CollisionState state) : entityA(entityA), entityB(entityB),
                                                                             state(state) {
        type = EventType::Collision;
    }
};

enum class PlayerAction { Attack, Interact, useItem, Jump };

struct PlayerActionEvent : BaseEvent {
    Entity *player = nullptr;
    PlayerAction action{};

    PlayerActionEvent(Entity *player, PlayerAction action) : player(player), action(action) {
        type = EventType::PlayerAction;
    }
};

enum class MouseInteractionState { Pressed, Released, Cancel };

struct MouseInteractionEvent : BaseEvent {
    Entity *entity = nullptr;
    MouseInteractionState state{};

    MouseInteractionEvent(Entity *entity, MouseInteractionState state) : entity(entity), state(state) {
        type = EventType::MouseInteraction;
    }
};

struct SpawnerChangeEvent : BaseEvent {
    Entity *entity = nullptr;

    ParticleType pType = ParticleType::Sand;

    SpawnerChangeEvent(ParticleType pType) {
        this->pType = pType;
        type = EventType::SpawnerChange;
    }
};

struct MenuToggleEvent : BaseEvent {
    MenuToggleEvent() {
        type = EventType::MenuToggle;
    }
};
#endif //INC_8051TUTORIAL_BASEEVENT_H
