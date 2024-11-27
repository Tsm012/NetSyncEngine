#include "UI.h"

bool UI::initialize(std::string title) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    return false;
  }

  window =
      SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }

  return true;
}

SDL_Rect UI::update() {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_UP:
          redBox.y -= moveStep;
          changed = true;
          break;
        case SDLK_DOWN:
          redBox.y += moveStep;
          changed = true;
          break;
        case SDLK_LEFT:
          redBox.x -= moveStep;
          changed = true;
          break;
        case SDLK_RIGHT:
          redBox.x += moveStep;
          changed = true;
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
    return redBox;
}

void UI::cleanup() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
