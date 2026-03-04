//
// Created by taylo on 1/7/2026.
//

#pragma once
#include <SDL3/SDL.h>

#include "ecs/World.h"
#include "manager/SceneManager.h"

class Game {
public:
    Game();

    ~Game();

    //these are the types SDL uses
    void init(const char *title, int width, int height, bool fullscreen);

    //game loop functions
    void handleEvents();

    void update(float deltaTime);

    void render();

    void destroy();

    bool running() {
        return isRunning;
    }

    SDL_Renderer *renderer = nullptr;

    SceneManager sceneManager;
    static std::function<void(std::string)> onSceneChangeRequest;

private:
    int frameCount = 0;
    bool isRunning = false;
    bool isGreen = true;

    SDL_Window *window = nullptr;
    SDL_Event event;
    //sdl stores colour channels as 8bit unsigned, 0-255
    Uint8 r, g, b, a;
};

