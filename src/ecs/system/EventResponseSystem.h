//
// Created by taylo on 3/11/2026.
//

#ifndef INC_8051TUTORIAL_EVENTRESPONSESYSTEM_H
#define INC_8051TUTORIAL_EVENTRESPONSESYSTEM_H
#include <functional>
#include "../event/BaseEvent.h"

class Entity;
class World;

class EventResponseSystem {
public:
    EventResponseSystem(World &world);

private:
    //collisions
    void onCollision(const CollisionEvent &e, const char *otherTag, World &world);

    bool getCollisionEntities(const CollisionEvent &e, const char *otherTag, Entity *&player, Entity *&other);

    //player action
    void onPlayerAction(const CollisionEvent &e,
                        const std::function<void(Entity *player, PlayerAction action)> &callback);

    void onMouseInteraction(const MouseInteractionEvent &e);

    void onSpawnerChangeEvent(const SpawnerChangeEvent &e, World &world);

    void onMenuToggle(const MenuToggleEvent &e, World &world);

    void onFireExtinguish(const FireExtinguishEvent &e, World &world);

    void onSpawnAudio(const SpawnAudioEvent &e, World &world);
};

#endif //INC_8051TUTORIAL_EVENTRESPONSESYSTEM_H
