//
// Created by taylo on 3/11/2026.
//
#include "MouseInputSystem.h"

#include "ParticleGrid.h"
#include "World.h"

void MouseInputSystem::update(World &world, const SDL_Event &event, ParticleGrid *grid) {
    if (event.type != SDL_EVENT_MOUSE_MOTION && event.type != SDL_EVENT_MOUSE_BUTTON_DOWN && event.type !=
        SDL_EVENT_MOUSE_BUTTON_UP && event.type != SDL_EVENT_MOUSE_WHEEL) {
        return;
    }
    float mx, my;
    SDL_GetMouseState(&mx, &my);
    //world.setMouseScreenPosition((int) mx, (int) my);
    bool clickedClickable = false;
    float camOffx = 0.0f, camOffy = 0.0f;
    float camZoom = 1.0f;
    BrushState *brushState = nullptr;
    for (auto &e: world.getEntities()) {
        //find the camera and brushState components, will be needed for spawning logic
        if (e->hasComponent<Camera>()) {
            auto &camera = e->getComponent<Camera>();
            camZoom = camera.zoom;
            camOffx = camera.view.x * camZoom;
            camOffy = camera.view.y * camZoom;
        }
        if (e->hasComponent<BrushState>()) {
            brushState = &e->getComponent<BrushState>();
            brushState->mouseScreenPos.x = mx;
            brushState->mouseScreenPos.y = my;
        }
        if (e->hasComponent<Clickable>() && e->hasComponent<Collider>()) {
            auto &clickable = e->getComponent<Clickable>();
            auto &collider = e->getComponent<Collider>();

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
    if (brushState == nullptr) return;
    if (event.type == SDL_EVENT_MOUSE_WHEEL) {
        if (event.wheel.y > 0) {
            brushState->brushSize++;
            if (brushState->brushSize > brushState->maxBrushSize) {
                brushState->brushSize = brushState->maxBrushSize;
            }
        } else if (event.wheel.y < 0) {
            brushState->brushSize--;
            if (brushState->brushSize < 0) {
                brushState->brushSize = 0;
            }
        }

        std::cout << "Brush radius: " << brushState->brushSize << std::endl;
        return;
    }

    if (grid == nullptr) return;
    if (brushState == nullptr) return;
    //set new brushState location with offsets to ensure spawning happens on mouse
    brushState->mouseScreenPos = Vector2D(mx, my);
    brushState->mouseWorldPos = Vector2D(
        static_cast<int>((mx + camOffx) / camZoom),
        static_cast<int>((my + camOffy) / camZoom)
    );

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
        event.button.button == SDL_BUTTON_LEFT) {
        //world.printParticleCounts(*grid);
        brushState->isPainting = !clickedClickable;
        brushState->uiCapturedClick = clickedClickable;
        if (brushState->isPainting) {
            world.getEventManager().emit(SpawnAudioEvent{brushState->selectedParticle, true});
        }
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
        event.button.button == SDL_BUTTON_LEFT) {
        if (brushState->isPainting) {
            world.getEventManager().emit(SpawnAudioEvent{brushState->selectedParticle, false});
        }
        brushState->isPainting = false;
        brushState->uiCapturedClick = false;
    }
}
