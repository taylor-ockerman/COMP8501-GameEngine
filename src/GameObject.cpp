//
// Created by taylo on 1/14/2026.
//
#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char *path, float x, float y) : xPos(x), yPos(y), texture(TextureManager::load(path)) {
}

GameObject::~GameObject() {
    if (texture) SDL_DestroyTexture(texture);
}

void GameObject::update(float deltaTime) {
    //move gameobject 1 pixel on x and y each frame
    //xPos++;
    //yPos++;
    float speed = 60.0f;
    xPos += speed * deltaTime;
    yPos += speed * deltaTime;
    //where do we want to crop the texture from?
    srcRect.x = srcRect.y = 0;
    //how much to crop
    srcRect.w = srcRect.h = 32;

    //where to draw it
    destRect.x = static_cast<float>(xPos);
    destRect.y = static_cast<float>(yPos);

    //what size to render?
    destRect.w = srcRect.w;
    destRect.h = srcRect.h;
}

void GameObject::draw() {
    TextureManager::draw(texture, &srcRect, &destRect);
}
