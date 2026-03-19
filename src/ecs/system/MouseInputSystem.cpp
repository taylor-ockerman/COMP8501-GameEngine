//
// Created by taylo on 3/11/2026.
//
#include "MouseInputSystem.h"
#include "../World.h"

void MouseInputSystem::update(World &world, const SDL_Event &event) {
    if (event.type != SDL_EVENT_MOUSE_MOTION && event.type != SDL_EVENT_MOUSE_BUTTON_DOWN && event.type !=
        SDL_EVENT_MOUSE_BUTTON_UP) {
        return;
    }

    float mx, my;
    SDL_GetMouseState(&mx, &my);
    bool clickedClickable = false;
    float camOffx = 0.0f, camOffy = 0.0f;
    for (auto &e: world.getEntities()) {
        if (e->hasComponent<Camera>()) {
            auto &camera = e->getComponent<Camera>();
            camOffx = camera.view.x;
            camOffy = camera.view.y;
        }
        if (e->hasComponent<Clickable>() && e->hasComponent<Collider>()) {
            Clickable &clickable = e->getComponent<Clickable>();
            Collider &collider = e->getComponent<Collider>();

            if (!collider.enabled) continue;

            bool inside = (mx >= collider.rect.x && mx <= collider.rect.x + collider.rect.w && my >= collider.rect.y
                           &&
                           my <= collider.rect.y + collider.rect.h);
            //Hover
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                if (!inside && clickable.pressed) {
                    world.getEventManager().emit(MouseInteractionEvent{e.get(), MouseInteractionState::Cancel});
                }
            }

            //Pressed
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (inside) {
                        clickable.pressed = true;
                        clickedClickable = true;
                        world.getEventManager().emit(MouseInteractionEvent{
                            e.get(), MouseInteractionState::Pressed
                        });
                    }
                }
            }

            //Released
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (inside) {
                        clickable.pressed = false;
                        world.getEventManager().emit(
                            MouseInteractionEvent{e.get(), MouseInteractionState::Released});
                    }
                }
            }
        }
    }
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
        event.button.button == SDL_BUTTON_LEFT &&
        !clickedClickable) {
        world.spawnMaterialAtLocation(mx + camOffx, my + camOffy, World::MaterialType::Sand);
    }
}
