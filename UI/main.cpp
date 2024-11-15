#include <Client.h>
#include <SDL.h>
#include <SDL_main.h>
#include <Server.h>
#include <chrono>
#include <iostream>
#include <optional>
#include <thread>

//#define SERVER

void runServer(Server &server) {
  if (server.start(1234)) {
    while (true) {
      server.receiveData();
    }
  }
}

int main(int argc, char *args[]) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    return 1;
  }

  // Create a window
  SDL_Window *window =
      SDL_CreateWindow("SDL Box Example", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    return 1;
  }

  // Create a renderer
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError()
              << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  // Set initial rectangle size and position
  SDL_Rect redBox = {100, 150, 100, 100}; // Fixed size for red box

  // Movement step size
  int moveStep = 10;

  bool running = true;
  SDL_Event event;

#ifdef SERVER
  Server server;
  std::thread serverThread(runServer, std::ref(server));
  while (running) {
    std::optional<std::string> message = server.getChannel().receive();
    if (message.has_value()) {
      std::cout << "helo " << *message;
      std::string msg = message.value();
      if (msg.compare(0, 4, "left") == 0 && msg.length() >= 4) {
        redBox.x -= moveStep;
      } else if (msg.compare(0, 5, "right") == 0 && msg.length() >= 5) {
        redBox.x += moveStep;
      } else if (msg.compare(0, 2, "up") == 0 && msg.length() >= 2) {
        redBox.y -= moveStep;
      } else if (msg.compare(0, 4, "down") == 0 && msg.length() >= 4) {
        redBox.y += moveStep;
      }
    }

    // Event handling
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
#else
  Client client;
  if (client.connect("localhost", 1234)) {
    while (running) {
      // Event handling
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          running = false;
        } else if (event.type == SDL_KEYDOWN) {
          switch (event.key.keysym.sym) {
          case SDLK_UP:
            redBox.y -= moveStep;
            client.sendData("up");
            break;
          case SDLK_DOWN:
            redBox.y += moveStep;
            client.sendData("down");
            break;
          case SDLK_LEFT:
            redBox.x -= moveStep;
            client.sendData("left");
            break;
          case SDLK_RIGHT:
            redBox.x += moveStep;
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
#endif

  // Clean up and exit
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

#ifdef SERVER
  serverThread.join();
#else
  // clientThread.join();
#endif

  return 0;
}
