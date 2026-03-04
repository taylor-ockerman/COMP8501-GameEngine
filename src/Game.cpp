//
// Created by taylo on 1/7/2026.
//
#include "Game.h"
#include "Map.h"

#include <iostream>

#include "manager/AssetManager.h"
std::function<void(std::string)> Game::onSceneChangeRequest;

Game::Game() {
}

Game::~Game() {
    destroy();
}

void Game::init(const char *title, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    //initialize DSL library
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) == 1) {
        std::cout << "Subsystem initialized!" << std::endl;
        window = SDL_CreateWindow(title, width, height, flags);
        if (window) {
            std::cout << "Window created!" << std::endl;
        }
        //windows will be direct3d
        renderer = SDL_CreateRenderer(window, nullptr);
        if (renderer) {
            std::cout << "Renderer created!" << std::endl;
        } else {
            std::cout << "Renderer could not be created!" << std::endl;
            return;
        }
        isRunning = true;
    } else {
        isRunning = false;
    }
    //load assets
    AssetManager::loadAnimation("player", "../assets/animations/human_animations.xml");
    AssetManager::loadAnimation("enemy", "../assets/animations/bird_animations.xml");

    //load scenes
    sceneManager.loadScene("level1", "../assets/map1.tmx", width, height);
    sceneManager.loadScene("level2", "../assets/map2.tmx", width, height);

    //start level 1
    sceneManager.changeSceneDeferred("level1");

    //resolve scene callback
    onSceneChangeRequest = [this](std::string sceneName) {
        if (sceneManager.currentScene->getName() == "level2" && sceneName == "level2") {
            std::cout << "You win!" << std::endl;
            isRunning = false;
            return;
        }
        if (sceneName == "gameover") {
            std::cout << "Game Over" << std::endl;
            isRunning = false;
            return;
        }
        sceneManager.changeSceneDeferred(sceneName);
    };
}

void Game::handleEvents() {
    //SDL listens to OS for input events internally, adds them to a queue
    //SDL_Event event; old local variable, now using private class wide variable

    //Check for next event, if an event is available it will remove from the queue and store in event object
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_EVENT_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
}

void Game::update(float deltaTime) {
    sceneManager.update(deltaTime, event);
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //every frame the renderer is cleared with the draw color;
    SDL_RenderClear(renderer);

    sceneManager.render();

    //swaps back buffer to the screen
    SDL_RenderPresent(renderer);;
}

void Game::destroy() {
    TextureManager::clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Game Destroyed" << std::endl;
}
