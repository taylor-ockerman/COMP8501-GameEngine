#include <iostream>

#include "Game.h"

//global variable
Game *game = nullptr;


int main() {
    const int FPS = 60;
    const int desiredFrameTime = 1000 / FPS;
    int actualFrameTime;
    //bool same = (a == b); // false
    float deltaTime = 0.0f;
    Uint64 ticks = 0;

    game = new Game();
    game->init("8051 Tutorial Engine", 800, 600, false);

    //game loop
    while (game->running()) {
        Uint64 currentTicks = SDL_GetTicks();
        deltaTime = (currentTicks - ticks) / 1000.0f;
        ticks = currentTicks;

        //std::cout << deltaTime << std::endl;
        game->handleEvents();
        game->update(deltaTime);

        game->render();

        actualFrameTime = SDL_GetTicks() - ticks; //elapsed time in ms it took for current frame

        //frame limiter, keeps game running at desired frame rate
        if (actualFrameTime < desiredFrameTime) {
            SDL_Delay(desiredFrameTime - actualFrameTime);
        }
    }

    delete game;
    return 0;
}
