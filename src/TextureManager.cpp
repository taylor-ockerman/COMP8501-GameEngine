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

void TextureManager::draw(SDL_Texture *tex, const SDL_FRect* src, const SDL_FRect* dest) {
    //draw the texture to the screen from vram
    SDL_RenderTexture(game->renderer, tex, src, dest);
}

void TextureManager::loadLabel(Label &label) {
    auto it = textures.find(label.textureCacheKey);
    if (it != textures.end()) {
        label.texture = it->second;
        return;
    }

    updateLabel(label);
}

void TextureManager::updateLabel(Label &label) {
    if (!label.dirty) return;
    if (label.texture) {
        SDL_DestroyTexture(label.texture);
        label.texture = nullptr;
    }

    SDL_Surface* tempSurface = TTF_RenderText_Blended(label.font, label.text.c_str(),label.text.size(),label.color);
    if (!tempSurface) {
        std::cout << "Failed to load image: " << label.textureCacheKey << std::endl;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, tempSurface);
    SDL_DestroySurface(tempSurface);

    if (!texture) {
        std::cerr << "Failed to create texture: " << label.textureCacheKey << std::endl;
    }

    float w, h;

    SDL_GetTextureSize(texture, &w, &h);
    label.dst.w = w;
    label.dst.h = h;

    label.texture = texture;
    textures[label.textureCacheKey] = texture;

    label.dirty = false; //clean
}
