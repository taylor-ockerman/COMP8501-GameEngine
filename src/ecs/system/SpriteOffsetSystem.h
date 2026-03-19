//
// Created by taylo on 3/18/2026.
//

#ifndef INC_8051TUTORIAL_SPRITEOFFSETSYSTEM_H
#define INC_8051TUTORIAL_SPRITEOFFSETSYSTEM_H
#include <memory>
#include <vector>

#include "Entity.h"


class World;

class SpriteOffsetSystem {
public:
   void update(World &world);

private:
   std::vector<Entity *> queryCollidableSprites(const std::vector<std::unique_ptr<Entity> > &entities);
};
#endif //INC_8051TUTORIAL_SPRITEOFFSETSYSTEM_H
