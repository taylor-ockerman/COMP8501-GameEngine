//
// Created by taylo on 1/14/2026.
//

#ifndef INC_8051TUTORIAL_TEXTUREMANAGER_H
#define INC_8051TUTORIAL_TEXTUREMANAGER_H


#include <string>
#include <unordered_map>

#include "SDL3_image/SDL_image.h"

class TextureManager {
private:
    static std::unordered_map<std::string, SDL_Texture *> textures;

public:
    static SDL_Texture *load(const char *path);

    //floating point Rect = FRect
    static void draw(SDL_Texture *tex, SDL_FRect src, SDL_FRect dest);

    static void clean();
};

#endif //INC_8051TUTORIAL_TEXTUREMANAGER_H
