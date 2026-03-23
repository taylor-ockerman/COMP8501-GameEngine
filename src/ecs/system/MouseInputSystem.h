//
// Created by taylo on 3/11/2026.
//

#ifndef INC_8051TUTORIAL_MOUSEINPUTSYSTEM_H
#define INC_8051TUTORIAL_MOUSEINPUTSYSTEM_H
#include <SDL3/SDL_events.h>

class ParticleGrid;
class World;

class MouseInputSystem {
public:
    void update(World &world, const SDL_Event &event, ParticleGrid *grid);
};

#endif //INC_8051TUTORIAL_MOUSEINPUTSYSTEM_H
