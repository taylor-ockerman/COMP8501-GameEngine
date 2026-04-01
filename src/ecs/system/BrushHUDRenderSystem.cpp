//
// Created by taylo on 4/1/2026.
//
#include "BrushHUDRenderSystem.h"
#include "World.h"

void BrushHUDRenderSystem::render(World &world, SDL_Renderer *renderer) {
    //if (!world.getShowBrushPreview()) return;
    if (!renderer) return;

    int centerX = world.getMouseScreenX();
    int centerY = world.getMouseScreenY();

    int brushSize = world.getBrushSize();

    const float radiusScale = 4.0f;
    const float baseRadius = 1.0f;

    float radius = brushSize * radiusScale + baseRadius;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);

    drawCircle(renderer, centerX, centerY, (int) radius);
}

void BrushHUDRenderSystem::drawCircle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    const int segments = 128;

    for (int i = 0; i < segments; i++) {
        float a1 = (2.0f * std::numbers::pi * i) / segments;
        float a2 = (2.0f * std::numbers::pi * (i + 1)) / segments;

        float x1 = cx + radius * std::cos(a1);
        float y1 = cy + radius * std::sin(a1);
        float x2 = cx + radius * std::cos(a2);
        float y2 = cy + radius * std::sin(a2);

        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }
}


