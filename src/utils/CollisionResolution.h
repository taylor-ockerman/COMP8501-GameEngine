//
// Created by taylo on 3/22/2026.
//

#ifndef INC_8051TUTORIAL_COLLISIONRESOLUTION_H
#define INC_8051TUTORIAL_COLLISIONRESOLUTION_H


class Entity;

class CollisionResolution {
public:
    static void resolvePlayerWall(Entity &player, Entity &wall);
};
#endif //INC_8051TUTORIAL_COLLISIONRESOLUTION_H
