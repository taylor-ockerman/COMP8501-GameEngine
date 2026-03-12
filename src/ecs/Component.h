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

#include "Entity.h"
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

enum class RenderLayer {
    World,
    UI
};

struct Sprite {
    SDL_Texture *texture = nullptr;
    SDL_FRect src{};
    SDL_FRect dst{};
    RenderLayer renderLayer = RenderLayer::World;
    bool visible = true;
};

struct Collider {
    std::string tag;
    SDL_FRect rect{};
    bool enabled = true;
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

struct Clickable {
    std::function<void()> onPressed{};
    std::function<void()> onReleased{};
    std::function<void()> onCancel{};
    bool pressed = false;
};

struct Parent {
    Entity *parent = nullptr;
};

struct Children {
    std::vector<Entity *> children{};
};
#endif //INC_8051TUTORIAL_COMPONENT_H
