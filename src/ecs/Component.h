//
// Created by taylo on 1/21/2026.
//

#ifndef INC_8051TUTORIAL_COMPONENT_H
#define INC_8051TUTORIAL_COMPONENT_H
#include <functional>
#include <SDL3/SDL_render.h>

#include "../utils/Vector2D.h"
#include <string>
#include <unordered_map>

#include "system/AnimationClip.h"

struct Transform {
    Vector2D position{};
    float rotation{};
    float scale{};
    Vector2D oldPosition{};
};

struct Velocity {
    Vector2D direction{};
    float speed{};
};

struct Sprite {
    SDL_Texture *texture = nullptr;
    SDL_FRect src{};
    SDL_FRect dst{};
};

struct Collider {
    std::string tag;
    SDL_FRect rect{};
};

struct Animation {
    std::unordered_map<std::string, AnimationClip> clips{};
    std::string currentClip;
    float time{}; //time is accumulated for the current frame
    int currentFrame{}; //index of the current frame in the clip
    float speed = 0.1f; //time per frame
    std::string lastMoveClip = "walk_down";
};

struct Camera {
    SDL_FRect view{};
    float worldWidth;
    float worldHeight;
};

struct TimedSpawner {
    float spawnInterval{};
    std::function<void()> spawnCallback;
    float timer{};
};

//our game state, might have multiple scenes
struct SceneState {
    int coinsCollected = 0;
};

struct PlayerTag {
};

struct ProjectileTag {
};

struct Health {
    int currentHealth{};
};
#endif //INC_8051TUTORIAL_COMPONENT_H
