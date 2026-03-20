//
// Created by taylo on 1/28/2026.
//

#ifndef INC_8051TUTORIAL_KEYBOARDINPUTSYSTEM_H
#define INC_8051TUTORIAL_KEYBOARDINPUTSYSTEM_H

#include <iostream>
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "../Component.h"
#include "../Entity.h"

class KeyboardInputSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity> > &entities, const SDL_Event &event) {
        for (auto &e: entities) {
            if (e->hasComponent<PlayerTag>() && e->hasComponent<Velocity>()) {
                auto &v = e->getComponent<Velocity>();
                Vector2D velVec = {v.direction.x * v.magnitude, v.direction.y * v.magnitude};
                if (event.type == SDL_EVENT_KEY_DOWN) {
                    switch (event.key.key) {
                        case SDLK_W:
                            //velVec.y = -200.0f;
                            break;
                        case SDLK_S:
                            //velVec.y = 200.0f;
                            break;
                        case SDLK_A:
                            velVec.x = -200.0f;
                            break;
                        case SDLK_D:
                            velVec.x = 200.0f;
                            break;
                        case SDLK_SPACE:
                            velVec.y = -200.0f;
                        default:
                            break;
                    }
                    float mag = std::sqrt(velVec.x * velVec.x + velVec.y * velVec.y);
                    if (mag > 0.001f) {
                        //could us Vector2D.normalize here but we already calculated the sqrt soo this is probably faster..
                        v.direction.x = velVec.x / mag;
                        v.direction.y = velVec.y / mag;
                        v.magnitude = mag;
                    }
                }

                if (event.type == SDL_EVENT_KEY_UP) {
                    switch (event.key.key) {
                        case SDLK_W:
                            //v.direction.y = 0;
                            break;
                        case SDLK_S:
                            //v.direction.y = 0;
                            break;
                        case SDLK_A:
                            velVec.x = 0;
                            break;
                        case SDLK_D:
                            velVec.x = 0;
                            break;
                        default:
                            break;
                    }
                    float mag = std::sqrt(velVec.x * velVec.x + velVec.y * velVec.y);
                    if (mag > 0.001f) {
                        v.direction = velVec.normalize();
                        v.magnitude = mag;
                    }
                }
            }
        }
    }
};

#endif //INC_8051TUTORIAL_KEYBOARDINPUTSYSTEM_H
