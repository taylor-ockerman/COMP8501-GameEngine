//
// Created by taylo on 3/22/2026.
//

#ifndef INC_8051TUTORIAL_COLLISIONRESOLUTION_H
#define INC_8051TUTORIAL_COLLISIONRESOLUTION_H
#include <SDL3/SDL_rect.h>


class Entity;

class CollisionResolution {
public:
    static void resolvePlayerWall(Entity &player, Entity &wall);

    static void resolvePlayerSolidParticle(Entity &player, const SDL_FRect &cellRect);
};
#endif //INC_8051TUTORIAL_COLLISIONRESOLUTION_H
