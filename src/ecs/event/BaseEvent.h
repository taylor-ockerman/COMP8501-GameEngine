//
// Created by taylo on 3/11/2026.
//

#ifndef INC_8051TUTORIAL_BASEEVENT_H
#define INC_8051TUTORIAL_BASEEVENT_H

#include "../Entity.h"

enum class EventType {
    Collision,
    PlayerAction
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
#endif //INC_8051TUTORIAL_BASEEVENT_H
