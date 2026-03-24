#include <iostream>

#include "Game.h"

//global variable
Game *game = nullptr;


int main() {
    const int FPS = 60;
    //const int desiredFrameTime = 1000 / FPS;
    const Uint64 desiredFrameTime = 1000000000ULL / FPS; // nanoseconds per frame
    int actualFrameTime;
    //bool same = (a == b); // false
    //float deltaTime = 0.0f;
    Uint64 ticks = 0;

    game = new Game();
    game->init("8051 Tutorial Engine", 800, 600, false);

    //game loop
    while (game->running()) {
        Uint64 currentTicks = SDL_GetTicksNS();
        float deltaTime = (currentTicks - ticks) / 1000000000.0f; // convert ns → seconds
        // deltaTime = (currentTicks - ticks) / 1000.0f;
        ticks = currentTicks;

        //std::cout << deltaTime << std::endl;
        game->handleEvents();
        game->update(deltaTime);

        game->render();

        actualFrameTime = SDL_GetTicksNS() - ticks; //elapsed time in ms it took for current frame
        if (actualFrameTime != 0) {
            SDL_SetWindowTitle(
                game->getWindow(),
                ("Sandy Balls [FPS: " + std::to_string((int) (1.0f / deltaTime)) +
                 " A: " + std::to_string((int) (1000000000.0 / actualFrameTime)) + "]")
                .c_str()
            );
        }
        // frame limiter
        if (desiredFrameTime > actualFrameTime) {
            SDL_Delay((desiredFrameTime - actualFrameTime) / 1000000); // ns → ms
        }
    }

    delete game;
    return 0;
}
