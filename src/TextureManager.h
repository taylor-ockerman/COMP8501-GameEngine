//
// Created by taylo on 1/14/2026.
//

#ifndef INC_8051TUTORIAL_TEXTUREMANAGER_H
#define INC_8051TUTORIAL_TEXTUREMANAGER_H


#include <string>
#include <unordered_map>

#include "Component.h"
#include "SDL3_image/SDL_image.h"

class Vector2D;

class TextureManager {
private:
    static std::unordered_map<std::string, SDL_Texture *> textures;

public:
    static SDL_Texture *load(const char *path);

    //floating point Rect = FRect
    static void draw(SDL_Texture *tex, const SDL_FRect *src, const SDL_FRect *dest);

    static void loadLabel(Label &label);

    static void updateLabel(Label &label);

    static void clean();

    static void drawCircle(Vector2D &center, float radius, Uint8 r, Uint8 g, Uint8 b);
};

#endif //INC_8051TUTORIAL_TEXTUREMANAGER_H
