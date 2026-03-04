//
// Created by taylo on 1/14/2026.
//

#include "TextureManager.h"
#include <iostream>
#include "Game.h"
//global variable, doesn't create any extra memory, just grabs the reference of this object
extern Game *game;

std::unordered_map<std::string, SDL_Texture *> TextureManager::textures;

void TextureManager::clean() {
    for (auto &tex: textures) {
        SDL_DestroyTexture(tex.second);
        tex.second = nullptr;
    }
    textures.clear();
}

SDL_Texture *TextureManager::load(const char *path) {
    //Caching: storing the result of some work so you don't have to repeat the work next time
    //Check if the texture already exists in the map
    auto it = textures.find(path);
    if (it != textures.end()) {
        return it->second;
    }
    //a surface represents an image in RAM (cpu mem)
    //do we want to do anything with the image before we store it in GPU
    SDL_Surface *tempSurface = IMG_Load(path);
    if (!tempSurface) {
        std::cout << "Failed to load image: " << path << std::endl;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, tempSurface);
    SDL_DestroySurface(tempSurface);

    //check if the texture creation is successful
    if (!texture) {
        std::cout << "Failed to create texture: " << path << std::endl;
        return nullptr;
    }
    //store the new texture in the cache
    textures[path] = texture;

    return texture;
}

void TextureManager::draw(SDL_Texture *tex, SDL_FRect src, SDL_FRect dest) {
    //draw the texture to the screen from vram
    SDL_RenderTexture(game->renderer, tex, &src, &dest);
}
