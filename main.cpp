#include <iostream>
#include <string>

#include "resource.hpp"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    // Init
    window = SDL_CreateWindow("Updater", screen_x, screen_y, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, nullptr);
    TTF_Init();

    initResource();

    int precentage = 0;
    while (true) {

        clearScreenAndRepaint();

        if (precentage < 100) precentage++;
        renderProgressBar(precentage);

        SDL_Delay(16);
        SDL_RenderPresent(renderer);
    }

    destroyResource();

    TTF_Quit();
    SDL_Quit();
    return 0;
}