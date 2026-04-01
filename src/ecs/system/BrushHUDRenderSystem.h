//
// Created by taylo on 4/1/2026.
//

#ifndef INC_8051TUTORIAL_BRUSHHUDRENDERSYSTEM_H
#define INC_8051TUTORIAL_BRUSHHUDRENDERSYSTEM_H
#include <SDL3/SDL_render.h>
class World;

class BrushHUDRenderSystem {
public:
    void render(World &world, SDL_Renderer *renderer);

private:
    void drawCircle(SDL_Renderer *renderer, int cx, int cy, int radius);
};
#endif //INC_8051TUTORIAL_BRUSHHUDRENDERSYSTEM_H
