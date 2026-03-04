//
// Created by taylo on 2/4/2026.
//

#ifndef INC_8051TUTORIAL_COLLISION_H
#define INC_8051TUTORIAL_COLLISION_H
#include <SDL3/SDL_rect.h>

#include "../ecs/Component.h"

class Collision {
public:
    //AABB = axis aligned bounding box
    static bool AABB(const SDL_FRect rectA, const SDL_FRect rectB);

    static bool AABB(const Collider &colA, const Collider &colB);
};
#endif //INC_8051TUTORIAL_COLLISION_H
