//
// Created by taylo on 3/23/2026.
//
#include "KeyboardInputSystem.h"

#include <cmath>
#include <iostream>
#include "World.h"


void KeyboardInputSystem::update(World &world, const std::vector<std::unique_ptr<Entity> > &entities,
                                 const SDL_Event &event) {
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
                        e->getComponent<Acceleration>().isGrounded = false;
                        break;
                    case SDLK_1:
                        world.setSelectedBrushTool(BrushTool::Sand);
                        world.getEventManager().emit(SpawnerChangeEvent{ParticleType::Sand});
                        std::cout << "Selected: Sand" << std::endl;
                        break;
                    case SDLK_2:
                        world.setSelectedBrushTool(BrushTool::Water);
                        world.getEventManager().emit(SpawnerChangeEvent{ParticleType::Water});
                        std::cout << "Selected: Water" << std::endl;
                        break;
                    case SDLK_3:
                        world.setSelectedBrushTool(BrushTool::Smoke);
                        world.getEventManager().emit(SpawnerChangeEvent{ParticleType::Smoke});
                        std::cout << "Selected: Smoke" << std::endl;
                        break;
                    case SDLK_4:
                        world.setSelectedBrushTool(BrushTool::Stone);
                        world.getEventManager().emit(SpawnerChangeEvent{ParticleType::Stone});
                        std::cout << "Selected: Stone" << std::endl;
                        break;
                    case SDLK_E:
                        world.setSelectedBrushTool(BrushTool::Erase);
                        world.getEventManager().emit(SpawnerChangeEvent{ParticleType::Empty});
                        std::cout << "Selected: Eraser" << std::endl;
                        break;
                    default:
                        break;
                }
                float mag = std::sqrt(velVec.x * velVec.x + velVec.y * velVec.y);
                if (mag > 0.001f) {
                    //could us Vector2D.normalize here but we already calculated the sqrt, so this is probably faster..
                    v.direction.x = velVec.x / mag;
                    v.direction.y = velVec.y / mag;
                    v.magnitude = mag;
                } else {
                    v.direction.x = 0.0f;
                    v.direction.y = 0.0f;
                    v.magnitude = 0.0f;
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
                } else {
                    v.direction.x = 0.0f;
                    v.direction.y = 0.0f;
                    v.magnitude = 0.0f;
                }
            }
        }
    }
}
