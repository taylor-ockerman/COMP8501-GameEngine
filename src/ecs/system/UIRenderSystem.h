//
// Created by taylo on 3/11/2026.
//

#ifndef INC_8051TUTORIAL_UIRENDERSYSTEM_H
#define INC_8051TUTORIAL_UIRENDERSYSTEM_H
#include <memory>

#include "Entity.h"
#include "Component.h"
class World;

class UIRenderSystem {
public:
    void render(World &world, const std::vector<std::unique_ptr<Entity> > &entities);

private:
    void drawBrush(World &world, const std::vector<std::unique_ptr<Entity> > &entities);
};

#endif //INC_8051TUTORIAL_UIRENDERSYSTEM_H
