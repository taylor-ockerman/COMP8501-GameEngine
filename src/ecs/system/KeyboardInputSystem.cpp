//
// Created by taylo on 3/23/2026.
//
#include "KeyboardInputSystem.h"

#include <cmath>
#include <iostream>

#include "Game.h"
#include "World.h"

extern Game *game;

void KeyboardInputSystem::update(World &world, const std::vector<std::unique_ptr<Entity> > &entities,
                                 const SDL_Event &event) {
    BrushState bs;
    for (auto &e: entities) {
        if (e->hasComponent<BrushState>()) {
            bs = e->getComponent<BrushState>();
            break;
        }
    }
    for (auto &e: entities) {
        if (e->hasComponent<PlayerTag>() && e->hasComponent<Velocity>()) {
            auto &v = e->getComponent<Velocity>();
            Vector2D velVec = {v.direction.x * v.magnitude, v.direction.y * v.magnitude};
            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
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
                        bs.selectedParticle = ParticleType::Sand;
                        world.getEventManager().emit(SpawnerChangeEvent{ParticleType::Sand});
                        std::cout << "Selected: Sand" << std::endl;
                        break;
                    case SDLK_2:
                        bs.selectedParticle = ParticleType::Water;
                        world.getEventManager().emit(SpawnerChangeEvent{ParticleType::Water});
                        std::cout << "Selected: Water" << std::endl;
                        break;
                    case SDLK_3:
                        bs.selectedParticle = ParticleType::Smoke;
                        world.getEventManager().emit(SpawnerChangeEvent{ParticleType::Smoke});
                        std::cout << "Selected: Smoke" << std::endl;
                        break;
                    case SDLK_4:
                        bs.selectedParticle = ParticleType::Stone;
                        world.getEventManager().emit(SpawnerChangeEvent{ParticleType::Stone});
                        std::cout << "Selected: Stone" << std::endl;
                        break;
                    case SDLK_5:
                        bs.selectedParticle = ParticleType::Wood;
                        world.getEventManager().emit(SpawnerChangeEvent{ParticleType::Wood});
                        std::cout << "Selected: Wood" << std::endl;
                        break;
                    case SDLK_6:
                        bs.selectedParticle = ParticleType::Steam;
                        world.getEventManager().emit(SpawnerChangeEvent{ParticleType::Steam});
                        std::cout << "Selected: Steam" << std::endl;
                        break;
                    case SDLK_7:
                        bs.selectedParticle = ParticleType::Oil;
                        world.getEventManager().emit(SpawnerChangeEvent{ParticleType::Oil});
                        std::cout << "Selected: Oil" << std::endl;
                        break;
                    case SDLK_8:
                        bs.selectedParticle = ParticleType::Fire;
                        world.getEventManager().emit(SpawnerChangeEvent{ParticleType::Fire});
                        std::cout << "Selected: Fire" << std::endl;
                        break;
                    case SDLK_E:
                        bs.selectedParticle = ParticleType::Erase;
                        world.getEventManager().emit(SpawnerChangeEvent{ParticleType::Erase});
                        std::cout << "Selected: Eraser" << std::endl;
                        break;
                    case SDLK_ESCAPE:
                        if (!event.key.repeat) {
                            world.getEventManager().emit(MenuToggleEvent{});
                        }
                        break;
                    case SDLK_F10:
                        game->destroy();
                        std::exit(0);
                        break;
                    default:
                        break;
                }
                float mag = std::sqrt(velVec.x * velVec.x + velVec.y * velVec.y);
                if (mag > 0.001f) {
                    //could us Vector2D.normalize here, but we already calculated the sqrt, so this is faster
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
