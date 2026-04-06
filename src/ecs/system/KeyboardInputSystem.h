//
// Created by taylo on 1/28/2026.
//

#ifndef INC_8051TUTORIAL_KEYBOARDINPUTSYSTEM_H
#define INC_8051TUTORIAL_KEYBOARDINPUTSYSTEM_H

#include <memory>
#include <SDL3/SDL_events.h>

#include "Component.h"
#include "Entity.h"

class World;

class KeyboardInputSystem {
public:
    void update(World& world, const std::vector<std::unique_ptr<Entity> > &entities, const SDL_Event &event);
};

#endif //INC_8051TUTORIAL_KEYBOARDINPUTSYSTEM_H
