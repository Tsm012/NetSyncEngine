#pragma once
#include <Client.h>
#include <Server.h>
#include <SDL.h>
#include <iostream>
#include <optional>

class UI {
public:
    bool initialize();
    void handleEvents(Client& client, bool& running);
    void handleEvents(Server& server, bool& running);
    void cleanup();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Rect redBox = {100, 150, 100, 100}; // Fixed size for red box
};
