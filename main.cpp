#pragma once
#include <iostream>
#include <string>
#include <thread>

#include "resource.hpp"
#include "message_queue.hpp"
#include "AMConfig.hpp"

// test func
void inputThreadFunc() {
    std::string userInput;
    while (true) {
        std::cout << "Please enter the message u want to send to amupdater" << std::endl;
        std::getline(std::cin, userInput);
        content_strings.push(userInput);
    }
}

int main(int argc, char* argv[]) {
    /*
        Config Init
    */
    AMConfig_Init();
    title_string = AMConfig_GetGameTitle();
    revision_string = "REV " + AMConfig_GetRevision();

    /*
        Graphics Init
    */
    SDL_Init(SDL_INIT_VIDEO);

    // Init
    window = SDL_CreateWindow("Updater", screen_x, screen_y, SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);
    renderer = SDL_CreateRenderer(window, nullptr);
    TTF_Init();

    initResource();

    // Debug: Test For Message Queue
    std::thread inputThread(inputThreadFunc);
    inputThread.detach();

    auto msgVector = content_strings.getAllMessagesAsVector();
    for (const auto& msg : msgVector) {
        std::cout << msg << std::endl;
    }

    while (true) {

        clearScreenAndRepaint();
        renderProgressBar();

        SDL_Delay(16);
        SDL_RenderPresent(renderer);
    }

    destroyResource();

    TTF_Quit();
    SDL_Quit();
    return 0;
}