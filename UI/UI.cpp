#include "UI.h"

bool UI::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("SDL Box Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true;
}

void UI::handleEvents(Client& client, bool& running) {
    SDL_Event event;
    int moveStep = 10;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        client.sendData("up");
                        break;
                    case SDLK_DOWN:
                        client.sendData("down");
                        break;
                    case SDLK_LEFT:
                        client.sendData("left");
                        break;
                    case SDLK_RIGHT:
                        client.sendData("right");
                        break;
                }
            }
        }

        // Set render draw color (white)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Clear the window
        SDL_RenderClear(renderer);

        // Set render draw color (red) for the red box
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &redBox);

        // Update the screen
        SDL_RenderPresent(renderer);

        // Delay to control frame rate (e.g., 60 FPS)
        SDL_Delay(16);
    }
}

void UI::handleEvents(Server& server, bool& running) {
    SDL_Event event;
    int moveStep = 10;

    while (running) {
        std::optional<std::string> message = server.getChannel().receive();
        if (message.has_value()) {
            if (*message == "left") {
                redBox.x -= moveStep;
            } else if (*message == "right") {
                redBox.x += moveStep;
            } else if (*message == "up") {
                redBox.y -= moveStep;
            } else if (*message == "down") {
                redBox.y += moveStep;
            }
            std::cout << "Main thread updated redBox position: " << redBox.x << ", " << redBox.y << std::endl;
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Set render draw color (white)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Clear the window
        SDL_RenderClear(renderer);

        // Set render draw color (red) for the red box
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &redBox);

        // Update the screen
        SDL_RenderPresent(renderer);

        // Delay to control frame rate (e.g., 60 FPS)
        SDL_Delay(16);
    }
}

void UI::cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
