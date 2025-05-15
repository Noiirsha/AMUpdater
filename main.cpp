/*
    OpenSSL is required
*/

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>
#include <thread>

#include "renderer.hpp"
#include "message_queue.hpp"
#include "AMConfig.hpp"
#include "updater_network.hpp"

#pragma comment(lib, "ws2_32.lib")

#ifdef _DEBUG
// test func
void inputThreadFunc() {
    std::string userInput;
    while (true) {
        std::cout << "Please enter the message u want to send to amupdater" << std::endl;
        std::getline(std::cin, userInput);
        content_strings.push(userInput);
    }
}
#endif

void startUpdateFunc() {
    updateNetwork.startNetworkUpdater();
}

int main(int argc, char* argv[]) {
    /*
        Config Init and Network Init
    */
    AMConfig_Init();
    title_string = AMConfig_GetGameTitle();
    revision_string = "REV " + AMConfig_GetRevision();

    updateNetwork.init(AMConfig_GetServer(), AMConfig_GetNetId(), AMConfig_GetSerial(), AMConfig_GetCountdown());

    /*
        Graphics Init
    */
    SDL_Init(SDL_INIT_VIDEO);

    // Init
    window = SDL_CreateWindow("Updater", screen_x, screen_y, SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);
    renderer = SDL_CreateRenderer(window, nullptr);
    TTF_Init();

    initResource();

#ifdef _DEBUG
    // Debug: Test For Message Queue
    std::thread inputThread(inputThreadFunc);
    inputThread.detach();
#endif // _DEBUG

    // Start Updater Network Process
    std::thread networkUpdater(startUpdateFunc);
    networkUpdater.detach();

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