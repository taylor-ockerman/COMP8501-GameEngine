//
// Created by taylo on 4/1/2026.
//

#ifndef INC_8051TUTORIAL_HUDSYSTEM_H
#define INC_8051TUTORIAL_HUDSYSTEM_H
#include <memory>
#include <sstream>
#include <vector>

#include "Component.h"
#include "Entity.h"
class World;

class HUDSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity> > &entities);
};
#endif //INC_8051TUTORIAL_HUDSYSTEM_H
