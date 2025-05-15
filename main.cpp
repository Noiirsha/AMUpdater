#include <iostream>
#include <string>

#include "resource.hpp"
#include "message_queue.hpp"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    // Init
    window = SDL_CreateWindow("Updater", screen_x, screen_y, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, nullptr);
    TTF_Init();

    initResource();

    MessageQueue queue;
    queue.push("System Initialization .......");
    queue.push("    Get Hops               : Completed");
    queue.push("    Router name resolution : Completed");
    queue.push("    Server name resolution : Completed");
    queue.push("    System setup           : In progress");
    queue.push("    AMauthd services       : Not Connected");

    auto msgVector = queue.getAllMessagesAsVector();
    for (const auto& msg : msgVector) {
        std::cout << msg << std::endl;
    }


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