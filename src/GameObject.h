//
// Created by taylo on 1/14/2026.
//

#ifndef INC_8051TUTORIAL_GAMEOBJECT_H
#define INC_8051TUTORIAL_GAMEOBJECT_H
#include "Game.h"

class GameObject {
public:
    GameObject(const char *path, float x, float y);

    ~GameObject();

    void update(float deltaTime);

    void draw();

private:
    float xPos{}, yPos{};

    SDL_Texture *texture = nullptr;
    SDL_FRect srcRect{}, destRect{};
};
#endif //INC_8051TUTORIAL_GAMEOBJECT_H
