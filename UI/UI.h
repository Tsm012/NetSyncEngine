#pragma once
#include <Client.h>
#include <Server.h>
#include <SDL.h>
#include <iostream>
#include <optional>

class UI {
public:
    bool initialize();
    void run(Client& client);
    void run(Server& server);
    void cleanup();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Rect redBox = {100, 150, 100, 100}; // Fixed size for red box
};
